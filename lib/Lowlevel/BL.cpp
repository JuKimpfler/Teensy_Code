#include "BL.h"
#include "RGB.h"
#include "IR.h"
#include "Robot.h"
#include "Mouse.h"
#include "Debug.h"

BLC BL;

elapsedMillis BL_Timer_RX;
elapsedMillis BL_Timer_TX;

// Non-blocking RX state: accumulates bytes until '$' terminator.
// Sized to hold "DAT:" prefix + 16-byte payload + safety margin.
static char  rxBuf[96];
static uint8_t rxPos = 0;

// Packet prefix used for P2P data frames sent over the ESP bridge.
// The bridge forwards only "DAT:" frames to the peer robot and routes
// "DBG:" frames to the debug monitor, preventing packet collisions.
static const char DAT_PREFIX[] = "DAT:";
static const uint8_t DAT_PREFIX_LEN = 4;

// Sentinel value sent when the start button is not pressed (no live data).
static const int NO_DATA_SENTINEL = 999;

// Timeout (ms) after which the link is considered down.
// 500 ms is sufficient for WLAN P2P; the original 3000 ms was too slow
// to react to connection loss during a game.
static const unsigned long BL_RX_TIMEOUT_MS = 500;

// TX interval (ms).  100 ms is appropriate for WLAN/bridge latency.
// The original 10 ms value flooded the ESP UART unnecessarily.
static const unsigned long BL_TX_INTERVAL_MS = 100;

// Parse a 3-digit decimal integer from payload[offset..offset+2].
static inline int parse3DigitInt(const char* payload, int offset) {
    char tmp[4] = { payload[offset], payload[offset+1], payload[offset+2], '\0' };
    return atoi(tmp);
}

void BLC::doRolle(){
    // ---- Non-blocking RX parser ----
    // Read all available bytes and accumulate until the '$' frame terminator.
    while (UART_Pixy.available() > 0) {
        char c = (char)UART_Pixy.read();

        // Skip CR/LF that may be inserted by the ESP bridge or terminal.
        if (c == '\r' || c == '\n') continue;

        if (c == '$') {
            // Frame complete – null-terminate and process.
            rxBuf[rxPos] = '\0';

            // Determine payload start: strip optional "DAT:" prefix.
            const char* payload = rxBuf;
            if (rxPos >= DAT_PREFIX_LEN &&
                strncmp(rxBuf, DAT_PREFIX, DAT_PREFIX_LEN) == 0) {
                payload = rxBuf + DAT_PREFIX_LEN;
            }

            // Accept frames whose payload is at least 16 characters.
            if (strlen(payload) >= 16) {
                decode(payload);
                BL_Timer_RX = 0;
            }
            // Reset buffer for the next frame regardless of validity.
            rxPos = 0;
        } else {
            // Guard against buffer overflow: reset on excessive input.
            if (rxPos < sizeof(rxBuf) - 1) {
                rxBuf[rxPos++] = c;
            } else {
                rxPos = 0; // discard garbage frame
            }
        }
    }

    // ---- Cyclic TX ----
    if (BL_Timer_TX > BL_TX_INTERVAL_MS) {
        bool started = digitalReadFast(Start_Port);
        int angle = started ? (int)(round(abs(Ball.Angle) + 180)) : NO_DATA_SENTINEL;
        int dist  = started ? (int)round(abs(Ball.Distance))      : NO_DATA_SENTINEL;
        int extra = 0;
        // "DAT:" prefix lets the ESP bridge identify and forward this frame
        // to the peer robot without confusing it with debug traffic.
        char sendMsg[32];
        snprintf(sendMsg, sizeof(sendMsg),
                 "DAT:0%03d0%03d0%1d0%1d0%03d$", angle, dist, 0, 0, extra);
        UART_Pixy.print(sendMsg);
        BL_Timer_TX = 0;
    }

    // ---- Link timeout → safe state ----
    if (BL_Timer_RX > BL_RX_TIMEOUT_MS) {
        Rolle = "N";
        SinglePlayer = true;
    }
}

void BLC::showRolle(){
    if (Rolle=="A")      { RGB.write(1,"R"); }
    else if (Rolle=="D") { RGB.write(1,"G"); }
    else                 { RGB.write(1,"B"); }
}

void BLC::decode(const char* payload){
    if (strlen(payload) < 16) return;

    // Frame layout (0-based positions within the 16-char payload):
    //  [0]     separator
    //  [1..3]  angle (3 digits)
    //  [4]     separator
    //  [5..7]  distance (3 digits)
    //  [8]     separator
    //  [9]     info1 (1 digit)
    //  [10]    separator
    //  [11]    info2 (1 digit)
    //  [12]    separator
    //  [13..15] extra (3 digits)
    int angle = parse3DigitInt(payload, 1);
    int dist  = parse3DigitInt(payload, 5);

    if (angle == NO_DATA_SENTINEL && dist == NO_DATA_SENTINEL) {
        SinglePlayer = true;
    } else {
        SinglePlayer = false;
        Ball.Angle_P2    = angle - 180;
        Ball.Distance_P2 = dist;
    }

    if (!SinglePlayer) {
        Rolle = (Ball.Distance_P2 < Ball.Distance) ? "D" : "A";
    } else {
        Rolle = "N";
    }
}
