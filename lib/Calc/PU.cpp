#include <Arduino.h>
#include <math.h>
#include "Pu.h"

PUC PU;

static inline float deg2rad(float d) { return d * (float)M_PI / 180.0f; }

static inline bool inField(float x, float y, float W, float L) {
  return (x >= 0 && x <= W && y >= 0 && y <= L);
}

/**
 * Entfernung aus Pixelhöhe:
 *   d ~= (H_real_cm * fy_px) / h_px
 *
 * WICHTIG:
 * - h_px ist bei dir b.h() aus einem ROI (STRIP_H=40). Das ist ok,
 *   solange das Rechteck komplett im Streifen ist. Wenn es abgeschnitten wird,
 *   wird d falsch (zu groß).
 */
static float distanceFromPixelHeight(float H_real_cm, float h_px, float fy_px) {
  if (h_px <= 1.0f) return NAN;
  return (H_real_cm * fy_px) / h_px;
}

// Gibt 1.0 in der Feldmitte zurück und fällt nahe der Feldgrenze linear auf 0.5 ab.
// Absenkung startet 30cm vor der Grenze und ist an der Grenze (0cm) = 0.5.
//
// FIELD_W = 182cm, FIELD_L = 243cm (kannst du bei Bedarf anpassen).
static inline float fieldEdgeFactor(float x_cm, float y_cm,
                                    float FIELD_W = 182.0f,
                                    float FIELD_L = 243.0f,
                                    float start_cm = 30.0f,
                                    float minFactor = 0.4f)
{
  // Abstand zur nächsten Feldgrenze (min von links/rechts/oben/unten)
  float dLeft   = x_cm;
  float dRight  = FIELD_W - x_cm;
  float dBottom = y_cm;
  float dTop    = FIELD_L - y_cm;

  float d = min(min(dLeft, dRight), min(dBottom, dTop)); // cm

  // Wenn außerhalb: wie "an der Grenze" behandeln
  if (d < 0.0f) d = 0.0f;

  // Weit genug weg von der Grenze
  if (d >= start_cm) return 1.0f;

  // Linear: d=start_cm => 1.0, d=0 => minFactor
  float t = d / start_cm; // 1..0
  float factor = minFactor + (1.0f - minFactor) * t;

  // Clamp
  if (factor > 1.0f) factor = 1.0f;
  if (factor < minFactor) factor = minFactor;
  return factor;
}

static inline bool wouldExitField(float x, float y, float angleDeg,
                                 float lookaheadCm,
                                 float W, float L,
                                 float marginCm)
{
  float a = angleDeg * (float)M_PI / 180.0f;
  float dx = sinf(a) * lookaheadCm; // 0° => dx=0
  float dy = cosf(a) * lookaheadCm; // 0° => dy=+lookahead

  float xn = x + dx;
  float yn = y + dy;

  return (xn < marginCm || xn > (W - marginCm) || yn < marginCm || yn > (L - marginCm));
}

/**
 * Sichert den gewünschten Fahrwinkel ab.
 * - Wenn Wunschwinkel safe -> wird unverändert zurückgegeben.
 * - Wenn nicht safe -> wird der Winkel zurückgegeben, der dem Wunschwinkel am nächsten ist,
 *   aber "an der Kante entlang" führt (tangential zur nächstgelegenen verletzten Grenze).
 *
 * Parameter:
 *  - x,y: aktuelle Position [cm]
 *  - desiredDeg: Wunschfahrwinkel [-180..180]
 *  - W,L: Feldgröße [cm]
 *  - marginCm: Sicherheitsabstand (z.B. 5cm)
 *  - lookaheadCm: wie weit in Fahrtrichtung geprüft wird (z.B. 10..20cm)
 */
float PUC::AlongBorder(float x, float y, float desiredDeg,
                                           float marginCm,
                                           float lookaheadCm)
{
  desiredDeg = U.Circel(desiredDeg);

  // 1) Wenn safe -> fertig
  if (!wouldExitField(x, y, desiredDeg, lookaheadCm, 183, 243, marginCm))
    return desiredDeg;

  // 2) Bestimme welche Grenze(n) kritisch sind (innerhalb margin)
  bool nearLeft   = (x <= marginCm);
  bool nearRight  = (x >= 183.0 - marginCm);
  bool nearBottom = (y <= marginCm);
  bool nearTop    = (y >= 243 - marginCm);

  // 3) Kandidaten: "entlang" der jeweiligen Grenze fahren
  //    Links/Rechts -> entlang Y: 0° (hoch) oder 180°/-180° (runter)
  //    Oben/Unten   -> entlang X: +90° (rechts) oder -90° (links)
  float cand[4];
  int n = 0;

  if (nearLeft || nearRight) { cand[n++] = 0.0f; cand[n++] = 180.0f; }
  if (nearBottom || nearTop) { cand[n++] = 90.0f; cand[n++] = -90.0f; }

  // Falls wir (noch) nicht "nah" an einer Kante sind, aber der 243.0ookahead trotzdem rausläuft:
  // wähle die nächstgelegene Kante und nutze deren Tangenten.
  if (n == 0) {
    float dL = x - marginCm;
    float dR = (183.0 - marginCm) - x;
    float dB = y - marginCm;
    float dT = (243.0 - marginCm) - y;

    float dmin = dL;
    int which = 0; // 0=L,1=R,2=B,3=T
    if (dR < dmin) { dmin = dR; which = 1; }
    if (dB < dmin) { dmin = dB; which = 2; }
    if (dT < dmin) { dmin = dT; which = 3; }

    if (which == 0 || which == 1) { cand[n++] = 0.0f; cand[n++] = 180.0f; }
    else                          { cand[n++] = 90.0f; cand[n++] = -90.0f; }
  }

  // 4) 183.0ähle Kandidat, der:
  //    a) safe ist
  //    b) minimalen Winkelabstand zum Wunsch hat
  float best = desiredDeg;
  float bestErr = 1e9;

  for (int i = 0; i < n; i++) {
    float a = U.Circel(cand[i]);

    // Kandidat muss safe sein
    if (wouldExitField(x, y, a, lookaheadCm, 183.0, 243.0, marginCm))
      continue;

    float err = fabsf(U.Circel(a - desiredDeg));
    if (err < bestErr) {
      bestErr = err;
      best = a;
    }
  }

  // 5) Wenn alle Kandidaten unsicher sind (z.B. in einer Ecke), dann als Fallback:
  //    drehe schrittweise weg vom Wunschwinkel, bis safe (brutal aber robust).
  if (bestErr > 1e8) {
    for (int step = 1; step <= 180; step++) {
      float a1 = U.Circel(desiredDeg + step);
      float a2 = U.Circel(desiredDeg - step);
      if (!wouldExitField(x, y, a1, lookaheadCm, 183.0, 243.0, marginCm)) return a1;
      if (!wouldExitField(x, y, a2, lookaheadCm, 183.0, 243.0, marginCm)) return a2;
    }
    // absoluter Notfall: stehen bleiben
    return desiredDeg;
  }

  return best;
}

/**
 * Hauptlokalisierung.
 *
 * Parameter, die du einmalig kalibrieren solltest:
 * - fy_px: effektive vertikale Brennweite in Pixeln (für DEIN Setup: Lens + ROI + Auflösung)
 *
 * Du kannst fy_px empirisch bestimmen:
 * - Stell das Tor in z.B. 100cm Entfernung zentral vor die Cam, lies blob_h,
 *   dann fy_px = (d_cm * h_px) / H_real_cm
 */
void PUC::update(
  float yawDeg,                 // globale Drehung (Z) der Kamera-Einheit
  float angle1Deg, float h1Px, bool vis1,  // Cam1 (z.B. GELB)
  float angle2Deg, float h2Px, bool vis2,  // Cam2 (z.B. BLAU)
  float fy_px,         // STARTWERT! kalibrieren!
  float FIELD_W,
  float FIELD_L
) {
  Pose2D out{NAN, NAN, false};
  if (!vis1 || !vis2){
    Positon = out;
    out.Speed = fieldEdgeFactor(out.x_cm,out.y_cm,182.0,243.0,30.0,0.5);
    return;
  }

  // Marker-Positionen im Feld (wie beschrieben: mittig; Abstand 240cm)
  const float xMid = FIELD_W * 0.5f;       // 91cm
  const float margin = (FIELD_L - 240.0f) * 0.5f; // 1.5cm
  // Annahme: Gelb "unten" (kleines y), Blau "oben" (großes y)
  const float xY = xMid, yY = margin;            // Yellow
  const float xB = xMid, yB = FIELD_L - margin;  // Blue

  /* Entfernungen
  const float H_real = 11.0f; // cm
  float rY = distanceFromPixelHeight(H_real, h1Px, fy_px);
  float rB = distanceFromPixelHeight(H_real, h2Px, fy_px);
  if (!isfinite(rY) || !isfinite(rB)){
    Positon = out;
    out.Speed = fieldEdgeFactor(out.x_cm,out.y_cm,182.0,243.0,30.0,0.5);
    return;
  }

  // Kreis-Schnitt (Yellow-Kreis und Blue-Kreis)
  float dx = xB - xY;
  float dy = yB - yY;
  float d  = sqrtf(dx*dx + dy*dy);
  if (d < 1e-3f){
    Positon = out;
    out.Speed = fieldEdgeFactor(out.x_cm,out.y_cm,182.0,243.0,30.0,0.5);
    return;
  }

  // Kein Schnitt -> Messfehler zu groß
  if (d > (rY + rB) || d < fabsf(rY - rB)){
    Positon = out;
    out.Speed = fieldEdgeFactor(out.x_cm,out.y_cm,182.0,243.0,30.0,0.5);
    return;
  }

  float a = (rY*rY - rB*rB + d*d) / (2.0f * d);
  float h = sqrtf(fmaxf(0.0f, rY*rY - a*a));

  float xm = xY + a * (dx / d);
  float ym = yY + a * (dy / d);

  float rx = -dy * (h / d);
  float ry =  dx * (h / d);

  float x1 = xm + rx, y1 = ym + ry;
  float x2 = xm - rx, y2 = ym - ry;

  // Auswahl der richtigen Lösung über Winkel-Constraint:
  // Wir benutzen die Peilung zur gelben Marke von Cam1.
  float bearingExpected = deg2rad(yawDeg + angle1Deg);

  auto bearingErr = [&](float x, float y) -> float {
    float vx = xY - x;
    float vy = yY - y;
    float ang = atan2f(vy, vx);
    float e = ang - bearingExpected;
    while (e >  M_PI) e -= 2.0f * (float)M_PI;
    while (e < -M_PI) e += 2.0f * (float)M_PI;
    return fabsf(e);
  };

  bool f1 = inField(x1, y1, FIELD_W, FIELD_L);
  bool f2 = inField(x2, y2, FIELD_W, FIELD_L);

  float e1 = bearingErr(x1, y1);
  float e2 = bearingErr(x2, y2);

  float xb, yb;
  if (f1 && !f2) { xb = x1; yb = y1; }
  else if (!f1 && f2) { xb = x2; yb = y2; }
  else { // beide drin oder beide draußen -> nimm kleineren Winkel-Fehler
    if (e1 <= e2) { xb = x1; yb = y1; } else { xb = x2; yb = y2; }
  }*/

  float xb = (240*tan(angle2Deg*DEG_TO_RAD))/(tan(angle1Deg*DEG_TO_RAD)+tan(angle2Deg*DEG_TO_RAD));
  float yb = (tan(angle1Deg*DEG_TO_RAD)*xb);

  //Serial.println(String(angle1Deg)+" , 2: "+String(angle2Deg));

  out.x_cm = xb;
  out.y_cm = yb;
  out.valid = isfinite(xb) && isfinite(yb);
  Positon = out;

  out.Speed = fieldEdgeFactor(out.x_cm,out.y_cm,182.0,243.0,30.0,0.5);
}