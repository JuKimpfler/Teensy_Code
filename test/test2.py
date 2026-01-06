def create_chart_tab(self):
        """Erstellt den Live Chart Tab"""
        tab = QWidget()
        main_layout = QHBoxLayout(tab)
        main_layout.setSpacing(15)
        
        # Linke Seite: Control Panel
        left_panel = QWidget()
        left_layout = QVBoxLayout(left_panel)
        left_layout.setSpacing(15)
        
        # Geräteauswahl
        device_group = QGroupBox("📡 Datenquelle")
        device_layout = QVBoxLayout()
        
        device_label = QLabel("Gerät auswählen:")
        device_label.setStyleSheet("font-weight: bold; font-size: 11pt; color: #1976D2;")
        device_layout.addWidget(device_label)
        
        self.chart_device_combo = QComboBox()
        self.chart_device_combo.addItem("Kein Gerät ausgewählt", None)
        self.chart_device_combo.setMinimumHeight(40)
        self.chart_device_combo.currentIndexChanged.connect(self.on_chart_device_changed)
        self.chart_device_combo.setStyleSheet("""
            QComboBox {
                border: 3px solid #2196F3;
                border-radius: 8px;
                padding: 8px 12px;
                font-size: 11pt;
                font-weight: bold;
                background-color: white;
            }
            QComboBox:hover {
                border: 3px solid #1976D2;
                background-color: #E3F2FD;
            }
            QComboBox::drop-down {
                border: none;
                width: 35px;
            }
        """)
        device_layout.addWidget(self.chart_device_combo)
        
        device_group.setLayout(device_layout)
        left_layout.addWidget(device_group)
        
        # Status & Steuerung
        control_group = QGroupBox("⚙️ Steuerung")
        control_layout = QVBoxLayout()
        control_layout.setSpacing(10)
        
        # Status Anzeige
        self.chart_status_label = QLabel("⚪ Warte auf Datenquelle...")
        self.chart_status_label.setStyleSheet("""
            QLabel {
                background-color: #f5f5f5;
                border: 2px solid #ddd;
                border-radius: 6px;
                padding: 10px;
                font-size: 10pt;
                font-weight: bold;
            }
        """)
        self.chart_status_label.setAlignment(Qt.AlignCenter)
        control_layout.addWidget(self.chart_status_label)
        
        # Pause Button
        self.pause_chart_btn = QPushButton("⏸️ Pause")
        self.pause_chart_btn.setCheckable(True)
        self.pause_chart_btn.setMinimumHeight(45)
        self.pause_chart_btn.setStyleSheet("""
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #FF9800, stop:1 #F57C00);
                color: white;
                font-weight: bold;
                font-size: 12pt;
                border: 3px solid #EF6C00;
                border-radius: 8px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #FFB74D, stop:1 #FF9800);
            }
            QPushButton:checked {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #F44336, stop:1 #D32F2F);
                border: 3px solid #C62828;
            }
            QPushButton:checked:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #E57373, stop:1 #F44336);
            }
            QPushButton:disabled {
                background: #BDBDBD;
                border: 3px solid #9E9E9E;
            }
        """)
        self.pause_chart_btn.clicked.connect(self.on_pause_chart)
        control_layout.addWidget(self.pause_chart_btn)
        
        # Clear Button
        clear_chart_btn = QPushButton("🗑️ Daten löschen")
        clear_chart_btn.setMinimumHeight(45)
        clear_chart_btn.setStyleSheet("""
            QPushButton {
                background-color: #757575;
                color: white;
                font-weight: bold;
                font-size: 11pt;
                border: 2px solid #616161;
                border-radius: 8px;
            }
            QPushButton:hover {
                background-color: #616161;
            }
            QPushButton:pressed {
                background-color: #424242;
            }
        """)
        clear_chart_btn.clicked.connect(self.clear_chart_data)
        control_layout.addWidget(clear_chart_btn)
        
        control_group.setLayout(control_layout)
        left_layout.addWidget(control_group)
        
        # Statistiken
        stats_group = QGroupBox("📊 Statistiken")
        stats_layout = QVBoxLayout()
        
        self.chart_stats_text = QTextEdit()
        self.chart_stats_text.setReadOnly(True)
        self.chart_stats_text.setMaximumHeight(150)
        self.chart_stats_text.setStyleSheet("""
            QTextEdit {
                background-color: #263238;
                color: #00E676;
                font-family: 'Courier New', monospace;
                font-size: 10pt;
                border: 2px solid #37474F;
                border-radius: 6px;
                padding: 8px;
            }
        """)
        self.chart_stats_text.setPlainText("Keine Daten\n━━━━━━━━━━━━━━━━")
        stats_layout.addWidget(self.chart_stats_text)
        
        stats_group.setLayout(stats_layout)
        left_layout.addWidget(stats_group)
        
        left_layout.addStretch()
        
        main_layout.addWidget(left_panel)
        
        # Rechte Seite: Chart Display
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)
        right_layout.setContentsMargins(0, 0, 0, 0)
        right_layout.setSpacing(10)
        
        # Chart Header
        chart_header = QWidget()
        chart_header.setStyleSheet("""
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 #1976D2, stop:1 #2196F3);
                border-radius: 8px;
            }
        """)
        chart_header_layout = QHBoxLayout(chart_header)
        chart_header_layout.setContentsMargins(20, 15, 20, 15)
        
        chart_title = QLabel("📈 Live Daten-Visualisierung")
        chart_title.setStyleSheet("""
            QLabel {
                color: white;
                font-size: 16pt;
                font-weight: bold;
                background: transparent;
            }
        """)
        chart_header_layout.addWidget(chart_title)
        
        chart_header_layout.addStretch()
        
        # Sample Counter
        self.chart_sample_label = QLabel("Samples: 0")
        self.chart_sample_label.setStyleSheet("""
            QLabel {
                color: white;
                font-size: 12pt;
                font-weight: bold;
                background-color: rgba(255, 255, 255, 0.2);
                border-radius: 6px;
                padding: 8px 15px;
            }
        """)
        chart_header_layout.addWidget(self.chart_sample_label)
        
        right_layout.addWidget(chart_header)
        
        # Chart Container mit Schatten-Effekt
        chart_container = QWidget()
        chart_container.setStyleSheet("""
            QWidget {
                background-color: white;
                border: 3px solid #E0E0E0;
                border-radius: 10px;
            }
        """)
        chart_container_layout = QVBoxLayout(chart_container)
        chart_container_layout.setContentsMargins(10, 10, 10, 10)
        
        # Chart erstellen
        self.chart = QChart()
        self.chart.setTitle("")
        self.chart.setAnimationOptions(QChart.SeriesAnimations)
        self.chart.legend().setVisible(True)
        self.chart.legend().setAlignment(Qt.AlignBottom)
        self.chart.setBackgroundBrush(QColor(255, 255, 255))
        self.chart.setTitleBrush(QColor(33, 33, 33))
        
        # Achsen erstellen
        self.chart_axis_x = QValueAxis()
        self.chart_axis_x.setTitleText("Zeit (Samples)")
        self.chart_axis_x.setLabelFormat("%d")
        self.chart_axis_x.setRange(0, 100)
        self.chart_axis_x.setTitleBrush(QColor(66, 66, 66))
        self.chart_axis_x.setLabelsBrush(QColor(66, 66, 66))
        
        self.chart_axis_y = QValueAxis()
        self.chart_axis_y.setTitleText("Wert")
        self.chart_axis_y.setLabelFormat("%.2f")
        self.chart_axis_y.setRange(0, 100)
        self.chart_axis_y.setTitleBrush(QColor(66, 66, 66))
        self.chart_axis_y.setLabelsBrush(QColor(66, 66, 66))
        
        self.chart.addAxis(self.chart_axis_x, Qt.AlignBottom)
        self.chart.addAxis(self.chart_axis_y, Qt.AlignLeft)
        
        # Chart View
        self.chart_view = QChartView(self.chart)
        self.chart_view.setRenderHint(QPainter.Antialiasing)
        self.chart_view.setStyleSheet("background-color: transparent; border: none;")
        
        chart_container_layout.addWidget(self.chart_view)
        right_layout.addWidget(chart_container)
        
        main_layout.addWidget(right_panel)
        
        # Proportionen setzen
        main_layout.setStretch(0, 1)  # Links: 25%
        main_layout.setStretch(1, 3)  # Rechts: 75%
        
        # Serien-Dictionary für verschiedene Datenpunkte
        self.chart_series = {}
        self.chart_paused = False
        
        # Farben für verschiedene Serien
        self.chart_colors = [
            QColor(33, 150, 243),   # Blau
            QColor(244, 67, 54),    # Rot
            QColor(76, 175, 80),    # Grün
            QColor(255, 152, 0),    # Orange
            QColor(156, 39, 176),   # Lila
            QColor(0, 188, 212),    # Cyan
            QColor(255, 235, 59),   # Gelb
            QColor(121, 85, 72),    # Braun
        ]
        
        return tab
    
    def on_chart_device_changed(self):
        """Wird aufgerufen wenn ein anderes Gerät ausgewählt wird"""
        device = self.chart_device_combo.currentData()
        
        if device is None:
            self.chart_status_label.setText("⚪ Warte auf Datenquelle...")
            self.chart_status_label.setStyleSheet("""
                QLabel {
                    background-color: #f5f5f5;
                    border: 2px solid #ddd;
                    border-radius: 6px;
                    padding: 10px;
                    font-size: 10pt;
                    font-weight: bold;
                }
            """)
        else:
            hostname = self.chart_device_combo.currentText()
            self.chart_status_label.setText(f"🟢 Empfange von: {hostname}")
            self.chart_status_label.setStyleSheet("""
                QLabel {
                    background-color: #C8E6C9;
                    border: 2px solid #4CAF50;
                    border-radius: 6px;
                    padding: 10px;
                    font-size: 10pt;
                    font-weight: bold;
                    color: #2E7D32;
                }
            """)
        
        # Daten löschen bei Gerätewechsel
        self.clear_chart_data()
    
    def on_pause_chart(self):
        """Pause/Resume Chart"""
        self.chart_paused = self.pause_chart_btn.isChecked()
        
        if self.chart_paused:
            self.pause_chart_btn.setText("▶️ Fortsetzen")
            self.statusBar().showMessage("Chart pausiert", 2000)
        else:
            self.pause_chart_btn.setText("⏸️ Pause")
            self.statusBar().showMessage("Chart aktiv", 2000)
    
    def on_chart_data(self, device_ip: str, data_dict: dict):
        """Verarbeitet neue Chart-Daten"""
        # Prüfe ob Chart pausiert
        if self.chart_paused:
            return
        
        # Prüfe ob Gerät ausgewählt
        selected_device = self.chart_device_combo.currentData()
        if selected_device is None or selected_device != device_ip:
            return
        
        # Statistiken aktualisieren
        stats_text = "📊 LIVE DATEN\n"
        stats_text += "━━━━━━━━━━━━━━━━\n"
        
        for name, value in sorted(data_dict.items()):
            stats_text += f"{name:.<12} {value:>8.2f}\n"
        
        stats_text += "━━━━━━━━━━━━━━━━\n"
        stats_text += f"Serien: {len(data_dict)}"
        
        self.chart_stats_text.setPlainText(stats_text)
        
        # Daten zum Buffer hinzufügen
        self.chart_time_index += 1
        print(f"Index: {self.chart_time_index}")
        
        for name, value in data_dict.items():
            # Serie erstellen falls nicht vorhanden
            if name not in self.chart_series:
                series = QLineSeries()
                series.setName(name)
                
                # Farbe zuweisen
                color_index = len(self.chart_series) % len(self.chart_colors)
                pen = QPen(self.chart_colors[color_index])
                pen.setWidth(3)
                series.setPen(pen)
                
                self.chart.addSeries(series)
                series.attachAxis(self.chart_axis_x)
                series.attachAxis(self.chart_axis_y)
                
                self.chart_series[name] = series
            
            # Datenpunkt hinzufügen
            self.chart_data_buffer[name].append((self.chart_time_index, value))
            print(f"{name}_data: {self.self.chart_data_buffer[name]}")
        # Sample Counter aktualisieren
        self.chart_sample_label.setText(f"Samples: {self.chart_time_index}")
        
        # Chart aktualisieren
        self.update_chart()
    
    def update_chart(self):
        """Aktualisiert das Chart mit den aktuellen Daten"""
        if not self.chart_series:
            return
        
        # Berechne Y-Achsen-Range
        all_values = []
        
        for name, series in self.chart_series.items():
           
            # Punkte zur Serie hinzufügen
            for time_idx, value in self.chart_data_buffer[name]:
                series.append(QPointF(time_idx, value))
                all_values.append(value)
        # X-Achse anpassen
        if self.chart_time_index > 100:
            self.chart_axis_x.setRange(self.chart_time_index - 100, self.chart_time_index)
        else:
            self.chart_axis_x.setRange(0, 100)
        
        # Y-Achse anpassen
        if all_values:
            min_val = min(all_values)
            max_val = max(all_values)
            margin = (max_val - min_val) * 0.1 if max_val != min_val else 1.0
            self.chart_axis_y.setRange(min_val - margin, max_val + margin)
    
    def clear_chart_data(self):
        """Löscht alle Chart-Daten"""
        # Serien entfernen
        for series in self.chart_series.values():
            self.chart.removeSeries(series)
        
        self.chart_series.clear()
        self.chart_data_buffer.clear()
        self.chart_time_index = 0
        self.chart_stats_text.setPlainText("Keine Daten\n━━━━━━━━━━━━━━━━")
        self.chart_sample_label.setText("Samples: 0")
        
        # Achsen zurücksetzen
        self.chart_axis_x.setRange(0, 100)
        self.chart_axis_y.setRange(0, 100)
        
        self.statusBar().showMessage("Chart-Daten gelöscht", 2000)
    