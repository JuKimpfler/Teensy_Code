#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
HC-05 Bluetooth Module Configurator
Umfassende GUI-Anwendung zur Konfiguration von HC-05 Modulen
"""

import sys
import serial
import serial.tools.list_ports
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTabWidget, QComboBox, QPushButton, QLabel, QLineEdit,
    QTextEdit, QGroupBox, QSpinBox, QCheckBox, QTableWidget,
    QTableWidgetItem, QMessageBox, QProgressBar, QRadioButton,
    QButtonGroup, QSplitter, QListWidget, QScrollArea
)
from PySide6.QtCore import Qt, QTimer, Signal, QThread
from PySide6.QtGui import QFont, QColor, QPalette
from typing import Optional, List, Dict
import time


class SerialWorker(QThread):
    """Worker-Thread für serielle Kommunikation"""
    response_received = Signal(str)
    error_occurred = Signal(str)
    
    def __init__(self, port: str, baudrate: int, command: str):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.command = command
        self.running = True
        self._serial = None
        
    def run(self):
        """Führe serielle Kommunikation mit umfassendem Error Handling aus"""
        try:
            # Validierung
            if not self.port:
                raise ValueError("Kein Port angegeben")
            if not self.command:
                raise ValueError("Kein Kommando angegeben")
            if self.baudrate not in [9600, 19200, 38400, 57600, 115200]:
                raise ValueError(f"Ungültige Baudrate: {self.baudrate}")
            
            # Serielle Verbindung öffnen
            try:
                self._serial = serial.Serial(
                    port=self.port,
                    baudrate=self.baudrate,
                    timeout=2,
                    write_timeout=2,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE
                )
            except serial.SerialException as e:
                raise serial.SerialException(f"Port {self.port} konnte nicht geöffnet werden: {str(e)}")
            except PermissionError:
                raise PermissionError(f"Keine Berechtigung für Port {self.port}. Port möglicherweise bereits in Verwendung.")
            
            time.sleep(0.1)
            
            # Kommando senden
            try:
                cmd = self.command if self.command.endswith('\r\n') else self.command + '\r\n'
                bytes_written = self._serial.write(cmd.encode('utf-8', errors='ignore'))
                if bytes_written == 0:
                    raise IOError("Konnte keine Daten senden")
                self._serial.flush()
            except serial.SerialTimeoutException:
                raise TimeoutError("Timeout beim Senden des Kommandos")
            except Exception as e:
                raise IOError(f"Fehler beim Senden: {str(e)}")
            
            time.sleep(0.5)
            
            # Antwort lesen
            response = ""
            max_attempts = 10
            attempts = 0
            
            try:
                while attempts < max_attempts:
                    if self._serial.in_waiting > 0:
                        try:
                            data = self._serial.read(self._serial.in_waiting)
                            response += data.decode('utf-8', errors='ignore')
                        except UnicodeDecodeError as e:
                            # Fallback auf Latin-1 bei Dekodierungsproblemen
                            response += data.decode('latin-1', errors='ignore')
                    
                    time.sleep(0.1)
                    attempts += 1
                    
                    # Beende wenn "OK" oder "ERROR" empfangen wurde
                    if "OK" in response or "ERROR" in response:
                        break
                        
            except serial.SerialException as e:
                raise serial.SerialException(f"Fehler beim Lesen: {str(e)}")
            
            # Response senden
            if response.strip():
                self.response_received.emit(response.strip())
            else:
                self.response_received.emit("Keine Antwort (Timeout nach 2 Sekunden)")
                
        except serial.SerialException as e:
            self.error_occurred.emit(f"Serieller Fehler: {str(e)}")
        except PermissionError as e:
            self.error_occurred.emit(f"Zugriffsfehler: {str(e)}")
        except TimeoutError as e:
            self.error_occurred.emit(f"Timeout: {str(e)}")
        except ValueError as e:
            self.error_occurred.emit(f"Ungültige Parameter: {str(e)}")
        except Exception as e:
            self.error_occurred.emit(f"Unerwarteter Fehler: {type(e).__name__} - {str(e)}")
        finally:
            # Sichere Bereinigung
            try:
                if self._serial and self._serial.is_open:
                    self._serial.close()
            except Exception:
                pass  # Ignoriere Fehler beim Schließen
                
    def stop(self):
        """Stoppe den Worker sicher"""
        self.running = False
        try:
            if self._serial and self._serial.is_open:
                self._serial.close()
        except Exception:
            pass


class HC05Configurator(QMainWindow):
    """Hauptfenster für HC-05 Konfiguration"""
    
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HC-05 Bluetooth Konfigurator v2.0")
        self.setGeometry(100, 100, 1200, 800)
        
        # Variablen
        self.serial_connections: Dict[str, serial.Serial] = {}
        self.current_worker: Optional[SerialWorker] = None
        self.terminal_serial: Optional[serial.Serial] = None
        self.terminal_connected: bool = False
        self.terminal_timer: Optional[QTimer] = None
        
        # AT-Kommando Referenz
        self.at_commands = {
            "Test": ("AT", "Verbindung testen"),
            "Version": ("AT+VERSION?", "Firmware-Version abfragen"),
            "Name abfragen": ("AT+NAME?", "Gerätename abfragen"),
            "Name setzen": ("AT+NAME=", "Gerätename setzen (Parameter: Name)"),
            "PIN abfragen": ("AT+PSWD?", "PIN-Code abfragen"),
            "PIN setzen": ("AT+PSWD=", "PIN-Code setzen (Parameter: 4-stellig)"),
            "Adresse abfragen": ("AT+ADDR?", "Bluetooth-Adresse abfragen"),
            "Rolle abfragen": ("AT+ROLE?", "Rolle abfragen (0=Slave, 1=Master)"),
            "Rolle setzen": ("AT+ROLE=", "Rolle setzen (Parameter: 0 oder 1)"),
            "Baudrate abfragen": ("AT+UART?", "UART-Konfiguration abfragen"),
            "Baudrate setzen": ("AT+UART=", "Baudrate setzen (z.B. 9600,0,0)"),
            "Verbinden": ("AT+LINK=", "Mit Gerät verbinden (Parameter: Adresse)"),
            "Verbindung trennen": ("AT+DISC", "Verbindung trennen"),
            "Geräte scannen": ("AT+INQM=1,9,48", "Inquiry-Modus setzen, dann AT+INQ"),
            "Scan starten": ("AT+INQ", "Bluetooth-Geräte scannen"),
            "Scan stoppen": ("AT+INQC", "Scan abbrechen"),
            "Status": ("AT+STATE?", "Verbindungsstatus abfragen"),
            "Reset": ("AT+RESET", "Modul zurücksetzen"),
            "Restore": ("AT+ORGL", "Werkseinstellungen wiederherstellen"),
            "Class abfragen": ("AT+CLASS?", "Geräteklasse abfragen"),
            "Class setzen": ("AT+CLASS=", "Geräteklasse setzen"),
            "IAC abfragen": ("AT+IAC?", "GIAC/LIAC abfragen"),
            "IAC setzen": ("AT+IAC=", "IAC setzen (9e8b33 oder 9e8b00)"),
            "Init": ("AT+INIT", "SPP-Profil initialisieren"),
            "SNiff Param": ("AT+SNIFF?", "Sniff-Parameter abfragen"),
            "Pairing Mode": ("AT+CMODE?", "Verbindungsmodus abfragen"),
            "Pairing Mode setzen": ("AT+CMODE=", "0=feste Adresse, 1=beliebig"),
        }
        
        # UI erstellen mit Error Handling
        try:
            self.init_ui()
        except Exception as e:
            QMessageBox.critical(
                None,
                "Initialisierungsfehler",
                f"Fehler beim Erstellen der Benutzeroberfläche:\n{str(e)}\n\nDie Anwendung wird beendet."
            )
            sys.exit(1)
        
        # Timer für Port-Aktualisierung
        try:
            self.port_timer = QTimer()
            self.port_timer.timeout.connect(self.refresh_ports)
            self.port_timer.start(2000)  # Alle 2 Sekunden
            
            self.refresh_ports()
        except Exception as e:
            self.statusBar().showMessage(f"Warnung: Port-Aktualisierung konnte nicht gestartet werden: {str(e)}", 5000)
        
    def init_ui(self):
        """Initialisiere Benutzeroberfläche"""
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        main_layout = QVBoxLayout(central_widget)
        
        # Tab-Widget
        self.tabs = QTabWidget()
        main_layout.addWidget(self.tabs)
        
        # Tabs erstellen
        self.create_setup_tab()
        self.create_at_commands_tab()
        self.create_terminal_tab()
        self.create_scanner_tab()
        self.create_batch_config_tab()
        
        # Statusleiste
        self.statusBar().showMessage("Bereit")
        
    def create_setup_tab(self):
        """Tab für Master-Slave Setup"""
        tab = QWidget()
        layout = QVBoxLayout(tab)
        
        # Titel
        title = QLabel("Master-Slave Verbindungssetup")
        title_font = QFont()
        title_font.setPointSize(14)
        title_font.setBold(True)
        title.setFont(title_font)
        layout.addWidget(title)
        
        # Master-Konfiguration
        master_group = QGroupBox("Master-Modul")
        master_layout = QVBoxLayout()
        
        master_port_layout = QHBoxLayout()
        master_port_layout.addWidget(QLabel("COM-Port:"))
        self.master_port_combo = QComboBox()
        master_port_layout.addWidget(self.master_port_combo)
        master_layout.addLayout(master_port_layout)
        
        master_baud_layout = QHBoxLayout()
        master_baud_layout.addWidget(QLabel("AT-Modus Baudrate:"))
        self.master_at_baud = QComboBox()
        self.master_at_baud.addItems(["9600", "38400", "57600", "115200"])
        self.master_at_baud.setCurrentText("38400")
        master_baud_layout.addWidget(self.master_at_baud)
        master_layout.addLayout(master_baud_layout)
        
        master_group.setLayout(master_layout)
        layout.addWidget(master_group)
        
        # Slave-Konfiguration
        slave_group = QGroupBox("Slave-Modul")
        slave_layout = QVBoxLayout()
        
        slave_port_layout = QHBoxLayout()
        slave_port_layout.addWidget(QLabel("COM-Port:"))
        self.slave_port_combo = QComboBox()
        slave_port_layout.addWidget(self.slave_port_combo)
        slave_layout.addLayout(slave_port_layout)
        
        slave_baud_layout = QHBoxLayout()
        slave_baud_layout.addWidget(QLabel("AT-Modus Baudrate:"))
        self.slave_at_baud = QComboBox()
        self.slave_at_baud.addItems(["9600", "38400", "57600", "115200"])
        self.slave_at_baud.setCurrentText("38400")
        slave_baud_layout.addWidget(self.slave_at_baud)
        slave_layout.addLayout(slave_baud_layout)
        
        slave_group.setLayout(slave_layout)
        layout.addWidget(slave_group)
        
        # Verbindungsparameter
        conn_group = QGroupBox("Verbindungsparameter")
        conn_layout = QVBoxLayout()
        
        # Betriebsmodus Baudrate
        data_baud_layout = QHBoxLayout()
        data_baud_layout.addWidget(QLabel("Daten-Baudrate (Betrieb):"))
        self.data_baudrate = QComboBox()
        self.data_baudrate.addItems(["9600", "19200", "38400", "57600", "115200"])
        self.data_baudrate.setCurrentText("9600")
        data_baud_layout.addWidget(self.data_baudrate)
        conn_layout.addLayout(data_baud_layout)
        
        # Namen
        name_layout = QHBoxLayout()
        name_layout.addWidget(QLabel("Master Name:"))
        self.master_name = QLineEdit("HC05_Master")
        name_layout.addWidget(self.master_name)
        name_layout.addWidget(QLabel("Slave Name:"))
        self.slave_name = QLineEdit("HC05_Slave")
        name_layout.addWidget(self.slave_name)
        conn_layout.addLayout(name_layout)
        
        # PIN
        pin_layout = QHBoxLayout()
        pin_layout.addWidget(QLabel("PIN-Code:"))
        self.pin_code = QLineEdit("1234")
        self.pin_code.setMaxLength(4)
        pin_layout.addWidget(self.pin_code)
        conn_layout.addLayout(pin_layout)
        
        conn_group.setLayout(conn_layout)
        layout.addWidget(conn_group)
        
        # Aktionen
        action_layout = QHBoxLayout()
        
        self.setup_btn = QPushButton("Automatisches Setup starten")
        self.setup_btn.clicked.connect(self.start_auto_setup)
        self.setup_btn.setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; font-weight: bold;")
        action_layout.addWidget(self.setup_btn)
        
        refresh_btn = QPushButton("Ports aktualisieren")
        refresh_btn.clicked.connect(self.refresh_ports)
        action_layout.addWidget(refresh_btn)
        
        layout.addLayout(action_layout)
        
        # Fortschrittsanzeige
        self.setup_progress = QProgressBar()
        layout.addWidget(self.setup_progress)
        
        # Log
        self.setup_log = QTextEdit()
        self.setup_log.setReadOnly(True)
        self.setup_log.setMaximumHeight(200)
        layout.addWidget(QLabel("Setup-Log:"))
        layout.addWidget(self.setup_log)
        
        layout.addStretch()
        self.tabs.addTab(tab, "Setup Manager")
        
    def create_at_commands_tab(self):
        """Tab für AT-Kommandos"""
        tab = QWidget()
        layout = QVBoxLayout(tab)
        
        # Port-Auswahl
        port_group = QGroupBox("Verbindungseinstellungen")
        port_layout = QHBoxLayout()
        
        port_layout.addWidget(QLabel("COM-Port:"))
        self.at_port_combo = QComboBox()
        port_layout.addWidget(self.at_port_combo)
        
        port_layout.addWidget(QLabel("Baudrate:"))
        self.at_baudrate = QComboBox()
        self.at_baudrate.addItems(["9600", "38400", "57600", "115200"])
        self.at_baudrate.setCurrentText("38400")
        port_layout.addWidget(self.at_baudrate)
        
        test_btn = QPushButton("Verbindung testen")
        test_btn.clicked.connect(self.test_connection)
        port_layout.addWidget(test_btn)
        
        port_group.setLayout(port_layout)
        layout.addWidget(port_group)
        
        # Splitter für Kommandoliste und Ausgabe
        splitter = QSplitter(Qt.Horizontal)
        
        # Linke Seite: Kommando-Liste
        left_widget = QWidget()
        left_layout = QVBoxLayout(left_widget)
        left_layout.addWidget(QLabel("Verfügbare AT-Kommandos:"))
        
        self.command_list = QListWidget()
        for cmd_name, (cmd, desc) in self.at_commands.items():
            self.command_list.addItem(f"{cmd_name}: {desc}")
        self.command_list.itemDoubleClicked.connect(self.load_command)
        left_layout.addWidget(self.command_list)
        
        splitter.addWidget(left_widget)
        
        # Rechte Seite: Kommando-Ausführung
        right_widget = QWidget()
        right_layout = QVBoxLayout(right_widget)
        
        # Kommando-Eingabe
        cmd_group = QGroupBox("Kommando ausführen")
        cmd_layout = QVBoxLayout()
        
        cmd_input_layout = QHBoxLayout()
        cmd_input_layout.addWidget(QLabel("AT-Kommando:"))
        self.at_command_input = QLineEdit()
        self.at_command_input.setPlaceholderText("z.B. AT+NAME? oder AT+NAME=MeinHC05")
        cmd_input_layout.addWidget(self.at_command_input)
        cmd_layout.addLayout(cmd_input_layout)
        
        btn_layout = QHBoxLayout()
        send_btn = QPushButton("Senden")
        send_btn.clicked.connect(self.send_at_command)
        send_btn.setStyleSheet("background-color: #2196F3; color: white; padding: 8px;")
        btn_layout.addWidget(send_btn)
        
        clear_btn = QPushButton("Log löschen")
        clear_btn.clicked.connect(lambda: self.at_response.clear())
        btn_layout.addWidget(clear_btn)
        
        cmd_layout.addLayout(btn_layout)
        cmd_group.setLayout(cmd_layout)
        right_layout.addWidget(cmd_group)
        
        # Antwort-Anzeige
        right_layout.addWidget(QLabel("Antworten:"))
        self.at_response = QTextEdit()
        self.at_response.setReadOnly(True)
        right_layout.addWidget(self.at_response)
        
        splitter.addWidget(right_widget)
        splitter.setSizes([400, 600])
        
        layout.addWidget(splitter)
        
        self.tabs.addTab(tab, "AT-Kommandos")
        
    def create_terminal_tab(self):
        """Terminal-Tab für direkte Kommunikation"""
        tab = QWidget()
        layout = QVBoxLayout(tab)
        
        # Verbindungseinstellungen
        conn_group = QGroupBox("Terminal-Verbindung")
        conn_layout = QHBoxLayout()
        
        conn_layout.addWidget(QLabel("Port:"))
        self.term_port = QComboBox()
        conn_layout.addWidget(self.term_port)
        
        conn_layout.addWidget(QLabel("Baudrate:"))
        self.term_baudrate = QComboBox()
        self.term_baudrate.addItems(["9600", "19200", "38400", "57600", "115200"])
        conn_layout.addWidget(self.term_baudrate)
        
        self.term_connect_btn = QPushButton("Verbinden")
        self.term_connect_btn.clicked.connect(self.toggle_terminal_connection)
        self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
        conn_layout.addWidget(self.term_connect_btn)
        
        conn_group.setLayout(conn_layout)
        layout.addWidget(conn_group)
        
        # Terminal-Ausgabe
        self.terminal_output = QTextEdit()
        self.terminal_output.setReadOnly(True)
        self.terminal_output.setStyleSheet("background-color: #1e1e1e; color: #00ff00; font-family: Consolas;")
        layout.addWidget(self.terminal_output)
        
        # Eingabe
        input_layout = QHBoxLayout()
        self.terminal_input = QLineEdit()
        self.terminal_input.setPlaceholderText("Kommando eingeben...")
        self.terminal_input.returnPressed.connect(self.send_terminal_command)
        input_layout.addWidget(self.terminal_input)
        
        send_term_btn = QPushButton("Senden")
        send_term_btn.clicked.connect(self.send_terminal_command)
        input_layout.addWidget(send_term_btn)
        
        layout.addLayout(input_layout)
        
        # Optionen
        options_layout = QHBoxLayout()
        self.term_add_cr = QCheckBox("CR hinzufügen")
        self.term_add_cr.setChecked(True)
        options_layout.addWidget(self.term_add_cr)
        
        self.term_add_lf = QCheckBox("LF hinzufügen")
        self.term_add_lf.setChecked(True)
        options_layout.addWidget(self.term_add_lf)
        
        clear_term_btn = QPushButton("Terminal löschen")
        clear_term_btn.clicked.connect(lambda: self.terminal_output.clear())
        options_layout.addWidget(clear_term_btn)
        
        options_layout.addStretch()
        layout.addLayout(options_layout)
        
        self.tabs.addTab(tab, "Terminal")
        self.terminal_connected = False
        
    def create_scanner_tab(self):
        """Tab für Bluetooth-Gerätescanner"""
        tab = QWidget()
        layout = QVBoxLayout(tab)
        
        layout.addWidget(QLabel("Bluetooth-Geräte Scanner"))
        
        # Einstellungen
        settings_layout = QHBoxLayout()
        settings_layout.addWidget(QLabel("Port:"))
        self.scanner_port = QComboBox()
        settings_layout.addWidget(self.scanner_port)
        
        settings_layout.addWidget(QLabel("Baudrate:"))
        self.scanner_baud = QComboBox()
        self.scanner_baud.addItems(["9600", "38400", "57600", "115200"])
        self.scanner_baud.setCurrentText("38400")
        settings_layout.addWidget(self.scanner_baud)
        
        self.scan_btn = QPushButton("Scan starten")
        self.scan_btn.clicked.connect(self.start_scan)
        self.scan_btn.setStyleSheet("background-color: #FF9800; color: white;")
        settings_layout.addWidget(self.scan_btn)
        
        settings_layout.addStretch()
        layout.addLayout(settings_layout)
        
        # Geräteliste
        self.device_table = QTableWidget()
        self.device_table.setColumnCount(3)
        self.device_table.setHorizontalHeaderLabels(["Adresse", "Klasse", "RSSI"])
        layout.addWidget(self.device_table)
        
        # Log
        layout.addWidget(QLabel("Scan-Log:"))
        self.scanner_log = QTextEdit()
        self.scanner_log.setReadOnly(True)
        self.scanner_log.setMaximumHeight(150)
        layout.addWidget(self.scanner_log)
        
        self.tabs.addTab(tab, "Geräte-Scanner")
        
    def create_batch_config_tab(self):
        """Tab für Batch-Konfiguration mehrerer Module"""
        tab = QWidget()
        layout = QVBoxLayout(tab)
        
        layout.addWidget(QLabel("Batch-Konfiguration (mehrere Module gleichzeitig)"))
        
        # Konfigurationsparameter
        params_group = QGroupBox("Gemeinsame Parameter")
        params_layout = QVBoxLayout()
        
        # Name-Präfix
        name_layout = QHBoxLayout()
        name_layout.addWidget(QLabel("Name-Präfix:"))
        self.batch_name_prefix = QLineEdit("HC05_")
        name_layout.addWidget(self.batch_name_prefix)
        self.batch_auto_number = QCheckBox("Auto-Nummerierung")
        self.batch_auto_number.setChecked(True)
        name_layout.addWidget(self.batch_auto_number)
        params_layout.addLayout(name_layout)
        
        # PIN
        pin_layout = QHBoxLayout()
        pin_layout.addWidget(QLabel("PIN:"))
        self.batch_pin = QLineEdit("1234")
        pin_layout.addWidget(self.batch_pin)
        params_layout.addLayout(pin_layout)
        
        # Baudrate
        baud_layout = QHBoxLayout()
        baud_layout.addWidget(QLabel("Daten-Baudrate:"))
        self.batch_baudrate = QComboBox()
        self.batch_baudrate.addItems(["9600", "19200", "38400", "57600", "115200"])
        baud_layout.addWidget(self.batch_baudrate)
        params_layout.addLayout(baud_layout)
        
        params_group.setLayout(params_layout)
        layout.addWidget(params_group)
        
        # Port-Auswahl
        ports_group = QGroupBox("Zu konfigurierende Ports")
        ports_layout = QVBoxLayout()
        
        self.batch_port_list = QListWidget()
        self.batch_port_list.setSelectionMode(QListWidget.MultiSelection)
        ports_layout.addWidget(self.batch_port_list)
        
        refresh_batch_btn = QPushButton("Ports aktualisieren")
        refresh_batch_btn.clicked.connect(self.refresh_batch_ports)
        ports_layout.addWidget(refresh_batch_btn)
        
        ports_group.setLayout(ports_layout)
        layout.addWidget(ports_group)
        
        # Aktionen
        action_layout = QHBoxLayout()
        self.batch_config_btn = QPushButton("Batch-Konfiguration starten")
        self.batch_config_btn.clicked.connect(self.start_batch_config)
        self.batch_config_btn.setStyleSheet("background-color: #9C27B0; color: white; padding: 10px;")
        action_layout.addWidget(self.batch_config_btn)
        layout.addLayout(action_layout)
        
        # Progress
        self.batch_progress = QProgressBar()
        layout.addWidget(self.batch_progress)
        
        # Log
        self.batch_log = QTextEdit()
        self.batch_log.setReadOnly(True)
        self.batch_log.setMaximumHeight(150)
        layout.addWidget(self.batch_log)
        
        self.tabs.addTab(tab, "Batch-Konfiguration")
        
    def refresh_ports(self):
        """Aktualisiere COM-Port-Listen mit Error Handling"""
        try:
            # Hole verfügbare Ports
            try:
                ports = [port.device for port in serial.tools.list_ports.comports()]
            except Exception as e:
                self.statusBar().showMessage(f"Fehler beim Laden der Ports: {str(e)}", 5000)
                return
            
            # Speichere aktuelle Auswahl
            current_selections = {}
            try:
                current_selections = {
                    'master': self.master_port_combo.currentText() if hasattr(self, 'master_port_combo') else '',
                    'slave': self.slave_port_combo.currentText() if hasattr(self, 'slave_port_combo') else '',
                    'at': self.at_port_combo.currentText() if hasattr(self, 'at_port_combo') else '',
                    'term': self.term_port.currentText() if hasattr(self, 'term_port') else '',
                    'scanner': self.scanner_port.currentText() if hasattr(self, 'scanner_port') else ''
                }
            except Exception:
                pass  # Ignoriere Fehler beim Speichern der Auswahl
            
            # Aktualisiere alle ComboBoxen
            try:
                for combo in [self.master_port_combo, self.slave_port_combo, 
                              self.at_port_combo, self.term_port, self.scanner_port]:
                    if combo is not None:
                        try:
                            combo.clear()
                            combo.addItems(ports)
                        except Exception:
                            pass
            except Exception:
                pass
                
            # Stelle Auswahl wieder her
            try:
                if current_selections.get('master') in ports:
                    self.master_port_combo.setCurrentText(current_selections['master'])
                if current_selections.get('slave') in ports:
                    self.slave_port_combo.setCurrentText(current_selections['slave'])
                if current_selections.get('at') in ports:
                    self.at_port_combo.setCurrentText(current_selections['at'])
                if current_selections.get('term') in ports:
                    self.term_port.setCurrentText(current_selections['term'])
                if current_selections.get('scanner') in ports:
                    self.scanner_port.setCurrentText(current_selections['scanner'])
            except Exception:
                pass
                
            # Aktualisiere Batch-Ports
            try:
                self.refresh_batch_ports()
            except Exception:
                pass
                
        except Exception as e:
            # Stiller Fehler - zeige nur in Statusleiste
            self.statusBar().showMessage(f"Warnung: Port-Aktualisierung fehlgeschlagen: {str(e)}", 3000)
            
    def refresh_batch_ports(self):
        """Aktualisiere Batch-Port-Liste mit Error Handling"""
        try:
            ports = [port.device for port in serial.tools.list_ports.comports()]
            
            if hasattr(self, 'batch_port_list') and self.batch_port_list is not None:
                # Speichere aktuelle Auswahl
                selected_ports = []
                try:
                    selected_ports = [item.text() for item in self.batch_port_list.selectedItems()]
                except Exception:
                    pass
                
                # Aktualisiere Liste
                self.batch_port_list.clear()
                self.batch_port_list.addItems(ports)
                
                # Stelle Auswahl wieder her
                for i in range(self.batch_port_list.count()):
                    item = self.batch_port_list.item(i)
                    if item and item.text() in selected_ports:
                        item.setSelected(True)
                        
        except Exception as e:
            # Stiller Fehler
            if hasattr(self, 'statusBar'):
                self.statusBar().showMessage(f"Batch-Port-Aktualisierung fehlgeschlagen: {str(e)}", 3000)
        
    def log_setup(self, message: str):
        """Schreibe in Setup-Log mit Error Handling"""
        try:
            timestamp = time.strftime('%H:%M:%S')
            self.setup_log.append(f"[{timestamp}] {message}")
            QApplication.processEvents()
        except Exception:
            # Stiller Fehler - Log ist nicht kritisch
            pass
        
    def send_at_command_sync(self, port: str, baudrate: int, command: str) -> str:
        """Sende AT-Kommando synchron mit umfassendem Error Handling"""
        ser = None
        try:
            # Validierung
            if not port:
                return "ERROR: Kein Port angegeben"
            if not command:
                return "ERROR: Kein Kommando angegeben"
            if baudrate not in [9600, 19200, 38400, 57600, 115200]:
                return f"ERROR: Ungültige Baudrate: {baudrate}"
            
            # Öffne serielle Verbindung
            try:
                ser = serial.Serial(
                    port=port,
                    baudrate=baudrate,
                    timeout=2,
                    write_timeout=2,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE
                )
            except serial.SerialException as e:
                return f"ERROR: Port {port} konnte nicht geöffnet werden - {str(e)}"
            except PermissionError:
                return f"ERROR: Keine Berechtigung für {port} (Port in Verwendung?)"
            except Exception as e:
                return f"ERROR: Unbekannter Fehler beim Öffnen von {port} - {str(e)}"
            
            time.sleep(0.1)
            
            # Sende Kommando
            try:
                cmd = command if command.endswith('\r\n') else command + '\r\n'
                bytes_written = ser.write(cmd.encode('utf-8', errors='ignore'))
                if bytes_written == 0:
                    return "ERROR: Konnte keine Daten senden"
                ser.flush()
            except serial.SerialTimeoutException:
                return "ERROR: Timeout beim Senden"
            except Exception as e:
                return f"ERROR: Fehler beim Senden - {str(e)}"
            
            time.sleep(0.5)
            
            # Lese Antwort
            response = ""
            max_attempts = 10
            attempts = 0
            
            try:
                while attempts < max_attempts:
                    if ser.in_waiting > 0:
                        try:
                            data = ser.read(ser.in_waiting)
                            response += data.decode('utf-8', errors='ignore')
                        except UnicodeDecodeError:
                            # Fallback auf Latin-1
                            response += data.decode('latin-1', errors='ignore')
                    
                    time.sleep(0.1)
                    attempts += 1
                    
                    # Beende wenn Antwort komplett
                    if "OK" in response or "ERROR" in response:
                        break
                        
            except serial.SerialException as e:
                return f"ERROR: Fehler beim Lesen - {str(e)}"
            except Exception as e:
                return f"ERROR: Unerwarteter Fehler beim Lesen - {str(e)}"
            
            return response.strip() if response.strip() else "Keine Antwort (Timeout)"
            
        except Exception as e:
            return f"ERROR: Unerwarteter Fehler - {type(e).__name__}: {str(e)}"
        finally:
            # Sichere Bereinigung
            if ser is not None:
                try:
                    if ser.is_open:
                        ser.close()
                except Exception:
                    pass  # Ignoriere Fehler beim Schließen
            
    def start_auto_setup(self):
        """Automatisches Master-Slave Setup mit robustem Error Handling"""
        # Validierung der Eingaben
        try:
            master_port = self.master_port_combo.currentText()
            slave_port = self.slave_port_combo.currentText()
            
            if not master_port or not slave_port:
                QMessageBox.warning(self, "Fehler", "Bitte wählen Sie beide COM-Ports aus!")
                return
                
            if master_port == slave_port:
                QMessageBox.warning(self, "Fehler", "Master und Slave müssen unterschiedliche Ports sein!")
                return
            
            # Prüfe ob Ports existieren
            available_ports = [port.device for port in serial.tools.list_ports.comports()]
            if master_port not in available_ports:
                QMessageBox.warning(self, "Fehler", f"Master-Port {master_port} nicht verfügbar!")
                return
            if slave_port not in available_ports:
                QMessageBox.warning(self, "Fehler", f"Slave-Port {slave_port} nicht verfügbar!")
                return
            
            # Validiere Namen
            master_name = self.master_name.text().strip()
            slave_name = self.slave_name.text().strip()
            if not master_name or not slave_name:
                QMessageBox.warning(self, "Fehler", "Namen dürfen nicht leer sein!")
                return
            if len(master_name) > 20 or len(slave_name) > 20:
                QMessageBox.warning(self, "Fehler", "Namen dürfen maximal 20 Zeichen lang sein!")
                return
            
            # Validiere PIN
            pin = self.pin_code.text().strip()
            if not pin or len(pin) != 4 or not pin.isdigit():
                QMessageBox.warning(self, "Fehler", "PIN muss genau 4 Ziffern enthalten!")
                return
                
        except Exception as e:
            QMessageBox.critical(self, "Validierungsfehler", f"Fehler bei der Eingabevalidierung:\n{str(e)}")
            return
            
        # Setup durchführen
        self.setup_btn.setEnabled(False)
        self.setup_log.clear()
        self.setup_progress.setValue(0)
        
        master_baud = int(self.master_at_baud.currentText())
        slave_baud = int(self.slave_at_baud.currentText())
        data_baud = self.data_baudrate.currentText()
        
        try:
            total_steps = 14
            current_step = 0
            
            # Schritt 1: Master testen
            self.log_setup("Teste Master-Verbindung...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, "AT")
            if "ERROR" in response and "OK" not in response:
                raise Exception(f"Master antwortet nicht korrekt: {response}")
            self.log_setup("✓ Master verbunden")
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 2: Slave testen
            self.log_setup("Teste Slave-Verbindung...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, "AT")
            if "ERROR" in response and "OK" not in response:
                raise Exception(f"Slave antwortet nicht korrekt: {response}")
            self.log_setup("✓ Slave verbunden")
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 3: Slave-Adresse auslesen
            self.log_setup("Lese Slave-Adresse aus...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, "AT+ADDR?")
            if "ERROR" in response or ":" not in response:
                raise Exception(f"Konnte Slave-Adresse nicht auslesen: {response}")
            
            try:
                slave_addr = response.split(":")[1].strip().split("\r")[0].split("\n")[0]
            except (IndexError, AttributeError):
                raise Exception(f"Ungültiges Adressformat: {response}")
            
            if not slave_addr or len(slave_addr) < 10:
                raise Exception(f"Ungültige Slave-Adresse: {slave_addr}")
                
            self.log_setup(f"✓ Slave-Adresse: {slave_addr}")
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 4: Slave auf Rolle 0 setzen
            self.log_setup("Konfiguriere Slave (Rolle 0)...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, "AT+ROLE=0")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Rolle-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 5: Slave-Name setzen
            self.log_setup(f"Setze Slave-Name: {slave_name}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, f"AT+NAME={slave_name}")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Namen-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 6: Slave-PIN setzen
            self.log_setup(f"Setze Slave-PIN: {pin}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, f"AT+PSWD={pin}")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei PIN-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 7: Slave-Baudrate setzen
            self.log_setup(f"Setze Slave-Baudrate: {data_baud}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(slave_port, slave_baud, f"AT+UART={data_baud},0,0")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Baudrate-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 8: Master auf Rolle 1 setzen
            self.log_setup("Konfiguriere Master (Rolle 1)...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, "AT+ROLE=1")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Rolle-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 9: Master-Name setzen
            self.log_setup(f"Setze Master-Name: {master_name}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, f"AT+NAME={master_name}")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Namen-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 10: Master-PIN setzen
            self.log_setup(f"Setze Master-PIN: {pin}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, f"AT+PSWD={pin}")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei PIN-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 11: Master-Baudrate setzen
            self.log_setup(f"Setze Master-Baudrate: {data_baud}...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, f"AT+UART={data_baud},0,0")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei Baudrate-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 12: Verbindungsmodus setzen
            self.log_setup("Setze Master-Verbindungsmodus...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(master_port, master_baud, "AT+CMODE=0")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei CMODE-Änderung: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 13: Slave-Adresse im Master hinterlegen
            self.log_setup(f"Verknüpfe Master mit Slave ({slave_addr})...")
            QApplication.processEvents()
            
            slave_addr_formatted = slave_addr.replace(":", ",")
            response = self.send_at_command_sync(master_port, master_baud, f"AT+BIND={slave_addr_formatted}")
            if "ERROR" in response and "OK" not in response:
                self.log_setup(f"Warnung bei BIND-Kommando: {response}")
            time.sleep(0.5)
            current_step += 1
            self.setup_progress.setValue(int(current_step / total_steps * 100))
            QApplication.processEvents()
            
            # Schritt 14: Fertig
            self.log_setup("=" * 50)
            self.log_setup("✓✓✓ SETUP ERFOLGREICH ABGESCHLOSSEN ✓✓✓")
            self.log_setup("=" * 50)
            self.log_setup(f"Master: {master_name} auf {master_port}")
            self.log_setup(f"Slave: {slave_name} auf {slave_port}")
            self.log_setup(f"Baudrate: {data_baud}")
            self.log_setup(f"PIN: {pin}")
            self.log_setup("")
            self.log_setup("Die Module sollten sich nun automatisch verbinden,")
            self.log_setup("wenn beide mit Strom versorgt werden.")
            
            current_step += 1
            self.setup_progress.setValue(100)
            QApplication.processEvents()
            
            QMessageBox.information(self, "Erfolg", 
                "Master-Slave Setup erfolgreich abgeschlossen!\n\n"
                "Entfernen Sie nun beide Module aus dem AT-Modus\n"
                "und starten Sie sie neu. Sie sollten sich automatisch verbinden.")
            
        except serial.SerialException as e:
            self.log_setup(f"✗ SERIELLER FEHLER: {str(e)}")
            QMessageBox.critical(self, "Serieller Fehler", 
                f"Setup fehlgeschlagen:\n{str(e)}\n\n"
                "Mögliche Ursachen:\n"
                "- Port ist bereits in Verwendung\n"
                "- Modul nicht angeschlossen\n"
                "- Falsche Baudrate gewählt")
        except ValueError as e:
            self.log_setup(f"✗ EINGABEFEHLER: {str(e)}")
            QMessageBox.critical(self, "Eingabefehler", f"Setup fehlgeschlagen:\n{str(e)}")
        except Exception as e:
            self.log_setup(f"✗ FEHLER: {type(e).__name__} - {str(e)}")
            QMessageBox.critical(self, "Fehler", 
                f"Setup fehlgeschlagen:\n{type(e).__name__}: {str(e)}\n\n"
                "Bitte überprüfen Sie:\n"
                "- Sind beide Module im AT-Modus?\n"
                "- Ist die Baudrate korrekt?\n"
                "- Sind die Module angeschlossen?")
        finally:
            self.setup_btn.setEnabled(True)
            QApplication.processEvents()
            
    def test_connection(self):
        """Teste AT-Verbindung"""
        port = self.at_port_combo.currentText()
        baudrate = int(self.at_baudrate.currentText())
        
        if not port:
            QMessageBox.warning(self, "Fehler", "Bitte wählen Sie einen Port aus!")
            return
            
        response = self.send_at_command_sync(port, baudrate, "AT")
        
        if "OK" in response:
            QMessageBox.information(self, "Erfolg", f"Verbindung erfolgreich!\n\nAntwort: {response}")
            self.at_response.append(f"[TEST] Verbindung zu {port} erfolgreich: {response}")
        else:
            QMessageBox.warning(self, "Fehler", f"Keine Verbindung!\n\nAntwort: {response}")
            self.at_response.append(f"[TEST] Verbindung zu {port} fehlgeschlagen: {response}")
            
    def load_command(self, item):
        """Lade Kommando in Eingabefeld"""
        text = item.text()
        cmd_name = text.split(":")[0].strip()
        
        if cmd_name in self.at_commands:
            cmd, _ = self.at_commands[cmd_name]
            self.at_command_input.setText(cmd)
            
    def send_at_command(self):
        """Sende AT-Kommando asynchron"""
        port = self.at_port_combo.currentText()
        baudrate = int(self.at_baudrate.currentText())
        command = self.at_command_input.text().strip()
        
        if not port or not command:
            QMessageBox.warning(self, "Fehler", "Port und Kommando müssen ausgefüllt sein!")
            return
            
        self.at_response.append(f"\n>>> Sende: {command}")
        
        # Worker-Thread starten
        self.current_worker = SerialWorker(port, baudrate, command)
        self.current_worker.response_received.connect(self.handle_at_response)
        self.current_worker.error_occurred.connect(self.handle_at_error)
        self.current_worker.start()
        
    def handle_at_response(self, response: str):
        """Behandle AT-Antwort"""
        self.at_response.append(f"<<< Antwort: {response}")
        self.statusBar().showMessage("Kommando erfolgreich gesendet", 3000)
        
    def handle_at_error(self, error: str):
        """Behandle AT-Fehler"""
        self.at_response.append(f"!!! Fehler: {error}")
        self.statusBar().showMessage("Fehler beim Senden", 3000)
        
    def toggle_terminal_connection(self):
        """Terminal-Verbindung umschalten mit Error Handling"""
        if not self.terminal_connected:
            # Verbinden
            port = self.term_port.currentText()
            
            if not port:
                QMessageBox.warning(self, "Fehler", "Bitte Port auswählen!")
                return
            
            try:
                baudrate = int(self.term_baudrate.currentText())
            except ValueError:
                QMessageBox.warning(self, "Fehler", "Ungültige Baudrate!")
                return
                
            try:
                # Prüfe ob Port verfügbar ist
                available_ports = [p.device for p in serial.tools.list_ports.comports()]
                if port not in available_ports:
                    QMessageBox.warning(self, "Fehler", f"Port {port} ist nicht verfügbar!")
                    return
                
                # Öffne serielle Verbindung
                self.terminal_serial = serial.Serial(
                    port=port,
                    baudrate=baudrate,
                    timeout=0.1,
                    write_timeout=1.0,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE
                )
                
                self.terminal_connected = True
                self.term_connect_btn.setText("Trennen")
                self.term_connect_btn.setStyleSheet("background-color: #f44336; color: white;")
                self.terminal_output.append(f"[Verbunden mit {port} @ {baudrate} baud]")
                self.statusBar().showMessage(f"Terminal verbunden mit {port}", 3000)
                
                # Timer für Empfang
                self.terminal_timer = QTimer()
                self.terminal_timer.timeout.connect(self.read_terminal_data)
                self.terminal_timer.start(100)
                
            except serial.SerialException as e:
                QMessageBox.critical(self, "Verbindungsfehler", 
                    f"Verbindung zu {port} fehlgeschlagen:\n{str(e)}\n\n"
                    "Mögliche Ursachen:\n"
                    "- Port bereits in Verwendung\n"
                    "- Gerät nicht angeschlossen\n"
                    "- Keine Berechtigung")
            except PermissionError:
                QMessageBox.critical(self, "Zugriffsfehler",
                    f"Keine Berechtigung für Port {port}.\n"
                    "Der Port wird möglicherweise von einem anderen Programm verwendet.")
            except Exception as e:
                QMessageBox.critical(self, "Fehler", 
                    f"Unerwarteter Fehler:\n{type(e).__name__}: {str(e)}")
        else:
            # Trennen
            try:
                if self.terminal_timer is not None:
                    self.terminal_timer.stop()
                    self.terminal_timer = None
                
                if self.terminal_serial is not None and self.terminal_serial.is_open:
                    self.terminal_serial.close()
                
                self.terminal_connected = False
                self.term_connect_btn.setText("Verbinden")
                self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
                self.terminal_output.append("[Verbindung getrennt]")
                self.statusBar().showMessage("Terminal getrennt", 3000)
                
            except Exception as e:
                # Auch bei Fehler Status zurücksetzen
                self.terminal_connected = False
                self.term_connect_btn.setText("Verbinden")
                self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
                self.terminal_output.append(f"[Fehler beim Trennen: {str(e)}]")
                QMessageBox.warning(self, "Warnung", f"Fehler beim Trennen:\n{str(e)}")
            
    def read_terminal_data(self):
        """Lese Daten vom Terminal mit Error Handling"""
        if not self.terminal_connected or self.terminal_serial is None:
            return
            
        try:
            if self.terminal_serial.in_waiting > 0:
                try:
                    data = self.terminal_serial.read(self.terminal_serial.in_waiting)
                    text = data.decode('utf-8', errors='ignore')
                    self.terminal_output.insertPlainText(text)
                    # Auto-scroll zum Ende
                    cursor = self.terminal_output.textCursor()
                    cursor.movePosition(cursor.End)
                    self.terminal_output.setTextCursor(cursor)
                except UnicodeDecodeError:
                    # Fallback auf Latin-1
                    text = data.decode('latin-1', errors='ignore')
                    self.terminal_output.insertPlainText(text)
                except Exception as e:
                    self.terminal_output.append(f"\n[Lesefehler: {str(e)}]")
                    
        except serial.SerialException as e:
            self.terminal_output.append(f"\n[Verbindung verloren: {str(e)}]")
            # Automatisch trennen bei Verbindungsverlust
            if self.terminal_timer:
                self.terminal_timer.stop()
            self.terminal_connected = False
            self.term_connect_btn.setText("Verbinden")
            self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
            self.statusBar().showMessage("Terminal-Verbindung verloren", 5000)
        except Exception as e:
            self.terminal_output.append(f"\n[Unerwarteter Fehler: {str(e)}]")
            
    def send_terminal_command(self):
        """Sende Terminal-Kommando mit Error Handling"""
        if not self.terminal_connected:
            QMessageBox.warning(self, "Fehler", "Nicht verbunden!")
            return
        
        if self.terminal_serial is None or not self.terminal_serial.is_open:
            QMessageBox.warning(self, "Fehler", "Serielle Verbindung nicht verfügbar!")
            self.terminal_connected = False
            self.term_connect_btn.setText("Verbinden")
            self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
            return
            
        try:
            command = self.terminal_input.text()
            
            # Füge CR/LF hinzu
            if self.term_add_cr.isChecked():
                command += '\r'
            if self.term_add_lf.isChecked():
                command += '\n'
            
            # Sende Kommando
            try:
                bytes_written = self.terminal_serial.write(command.encode('utf-8', errors='ignore'))
                self.terminal_serial.flush()
                
                if bytes_written == 0:
                    raise IOError("Keine Daten gesendet")
                    
                # Zeige gesendetes Kommando an
                self.terminal_output.append(f"\n>>> {self.terminal_input.text()}")
                self.terminal_input.clear()
                
            except serial.SerialTimeoutException:
                QMessageBox.warning(self, "Timeout", "Timeout beim Senden des Kommandos!")
            except UnicodeEncodeError as e:
                QMessageBox.warning(self, "Kodierungsfehler", 
                    f"Kommando enthält ungültige Zeichen:\n{str(e)}")
                    
        except serial.SerialException as e:
            QMessageBox.critical(self, "Verbindungsfehler", 
                f"Fehler beim Senden:\n{str(e)}\n\nVerbindung wird getrennt.")
            # Trennen
            if self.terminal_timer:
                self.terminal_timer.stop()
            self.terminal_connected = False
            self.term_connect_btn.setText("Verbinden")
            self.term_connect_btn.setStyleSheet("background-color: #4CAF50; color: white;")
        except Exception as e:
            QMessageBox.critical(self, "Fehler", 
                f"Unerwarteter Fehler beim Senden:\n{type(e).__name__}: {str(e)}")
        
    def start_scan(self):
        """Starte Bluetooth-Scan mit Error Handling"""
        port = self.scanner_port.currentText()
        
        if not port:
            QMessageBox.warning(self, "Fehler", "Bitte Port auswählen!")
            return
        
        try:
            baudrate = int(self.scanner_baud.currentText())
        except ValueError:
            QMessageBox.warning(self, "Fehler", "Ungültige Baudrate!")
            return
        
        # Prüfe Port-Verfügbarkeit
        available_ports = [p.device for p in serial.tools.list_ports.comports()]
        if port not in available_ports:
            QMessageBox.warning(self, "Fehler", f"Port {port} ist nicht verfügbar!")
            return
            
        self.scanner_log.clear()
        self.device_table.setRowCount(0)
        self.scan_btn.setEnabled(False)
        
        try:
            self.scanner_log.append("Initialisiere Scan...")
            QApplication.processEvents()
            
            # Inquiry-Modus setzen
            response = self.send_at_command_sync(port, baudrate, "AT+INQM=1,9,48")
            self.scanner_log.append(f"Inquiry-Modus: {response}")
            
            if "ERROR" in response and "OK" not in response:
                raise Exception(f"Konnte Inquiry-Modus nicht setzen: {response}")
            
            time.sleep(0.5)
            QApplication.processEvents()
            
            # Scan starten
            self.scanner_log.append("Starte Scan (30 Sekunden)...")
            self.scanner_log.append("Bitte warten...")
            QApplication.processEvents()
            
            response = self.send_at_command_sync(port, baudrate, "AT+INQ")
            
            if "ERROR" in response and "+INQ:" not in response:
                raise Exception(f"Scan konnte nicht gestartet werden: {response}")
            
            # Parse Geräte
            devices = []
            try:
                for line in response.split('\n'):
                    if '+INQ:' in line:
                        try:
                            parts = line.split(':')[1].split(',')
                            if len(parts) >= 3:
                                addr = parts[0].strip()
                                dev_class = parts[1].strip()
                                rssi = parts[2].strip()
                                devices.append((addr, dev_class, rssi))
                        except (IndexError, ValueError) as e:
                            self.scanner_log.append(f"Warnung: Ungültiger Eintrag: {line}")
                            
            except Exception as e:
                self.scanner_log.append(f"Warnung beim Parsen: {str(e)}")
                    
            # Tabelle füllen
            try:
                self.device_table.setRowCount(len(devices))
                for i, (addr, dev_class, rssi) in enumerate(devices):
                    self.device_table.setItem(i, 0, QTableWidgetItem(addr))
                    self.device_table.setItem(i, 1, QTableWidgetItem(dev_class))
                    self.device_table.setItem(i, 2, QTableWidgetItem(rssi))
            except Exception as e:
                raise Exception(f"Fehler beim Füllen der Tabelle: {str(e)}")
                
            self.scanner_log.append(f"Scan abgeschlossen. {len(devices)} Geräte gefunden.")
            
            if len(devices) == 0:
                self.scanner_log.append("Tipp: Stellen Sie sicher, dass:")
                self.scanner_log.append("- Das Modul im AT-Modus ist")
                self.scanner_log.append("- Bluetooth-Geräte in der Nähe eingeschaltet sind")
                self.scanner_log.append("- Die Geräte sichtbar/auffindbar sind")
            
        except serial.SerialException as e:
            self.scanner_log.append(f"Serieller Fehler: {str(e)}")
            QMessageBox.critical(self, "Verbindungsfehler", 
                f"Scan fehlgeschlagen:\n{str(e)}\n\n"
                "Überprüfen Sie die Verbindung zum Modul.")
        except Exception as e:
            self.scanner_log.append(f"Fehler: {str(e)}")
            QMessageBox.critical(self, "Fehler", 
                f"Scan fehlgeschlagen:\n{str(e)}\n\n"
                "Stellen Sie sicher, dass das Modul im AT-Modus ist.")
        finally:
            self.scan_btn.setEnabled(True)
            
    def start_batch_config(self):
        """Starte Batch-Konfiguration mit Error Handling"""
        selected_items = self.batch_port_list.selectedItems()
        
        if not selected_items:
            QMessageBox.warning(self, "Fehler", "Bitte wählen Sie mindestens einen Port aus!")
            return
        
        # Validierung
        try:
            name_prefix = self.batch_name_prefix.text().strip()
            if not name_prefix:
                QMessageBox.warning(self, "Fehler", "Name-Präfix darf nicht leer sein!")
                return
            if len(name_prefix) > 15:
                QMessageBox.warning(self, "Fehler", "Name-Präfix zu lang (max. 15 Zeichen)!")
                return
            
            pin = self.batch_pin.text().strip()
            if not pin or len(pin) != 4 or not pin.isdigit():
                QMessageBox.warning(self, "Fehler", "PIN muss genau 4 Ziffern enthalten!")
                return
            
            baudrate = self.batch_baudrate.currentText()
            if not baudrate:
                QMessageBox.warning(self, "Fehler", "Bitte Baudrate auswählen!")
                return
                
        except Exception as e:
            QMessageBox.critical(self, "Validierungsfehler", 
                f"Fehler bei der Eingabevalidierung:\n{str(e)}")
            return
            
        ports = [item.text() for item in selected_items]
        
        # Prüfe Port-Verfügbarkeit
        available_ports = [p.device for p in serial.tools.list_ports.comports()]
        unavailable = [p for p in ports if p not in available_ports]
        if unavailable:
            QMessageBox.warning(self, "Warnung", 
                f"Folgende Ports sind nicht verfügbar:\n" + "\n".join(unavailable) +
                "\n\nDiese werden übersprungen.")
            ports = [p for p in ports if p in available_ports]
        
        if not ports:
            QMessageBox.warning(self, "Fehler", "Keine verfügbaren Ports ausgewählt!")
            return
        
        self.batch_log.clear()
        self.batch_progress.setValue(0)
        self.batch_config_btn.setEnabled(False)
        
        success_count = 0
        fail_count = 0
        
        try:
            total = len(ports)
            
            for i, port in enumerate(ports):
                try:
                    self.batch_log.append(f"\n{'='*50}")
                    self.batch_log.append(f"Konfiguriere Port {i+1}/{total}: {port}")
                    self.batch_log.append(f"{'='*50}")
                    QApplication.processEvents()
                    
                    # Name generieren
                    if self.batch_auto_number.isChecked():
                        name = f"{name_prefix}{i+1}"
                    else:
                        name = name_prefix
                    
                    # Längenprüfung
                    if len(name) > 20:
                        self.batch_log.append(f"✗ Name zu lang: {name} (max. 20 Zeichen)")
                        fail_count += 1
                        continue
                        
                    # Test
                    response = self.send_at_command_sync(port, 38400, "AT")
                    if "ERROR" in response and "OK" not in response:
                        self.batch_log.append(f"✗ Modul antwortet nicht korrekt: {response}")
                        fail_count += 1
                        continue
                        
                    # Name setzen
                    response = self.send_at_command_sync(port, 38400, f"AT+NAME={name}")
                    if "ERROR" in response and "OK" not in response:
                        self.batch_log.append(f"⚠ Warnung bei Namen: {response}")
                    else:
                        self.batch_log.append(f"✓ Name gesetzt: {name}")
                    time.sleep(0.3)
                    QApplication.processEvents()
                    
                    # PIN setzen
                    response = self.send_at_command_sync(port, 38400, f"AT+PSWD={pin}")
                    if "ERROR" in response and "OK" not in response:
                        self.batch_log.append(f"⚠ Warnung bei PIN: {response}")
                    else:
                        self.batch_log.append(f"✓ PIN gesetzt: {pin}")
                    time.sleep(0.3)
                    QApplication.processEvents()
                    
                    # Baudrate setzen
                    response = self.send_at_command_sync(port, 38400, f"AT+UART={baudrate},0,0")
                    if "ERROR" in response and "OK" not in response:
                        self.batch_log.append(f"⚠ Warnung bei Baudrate: {response}")
                    else:
                        self.batch_log.append(f"✓ Baudrate gesetzt: {baudrate}")
                    time.sleep(0.3)
                    QApplication.processEvents()
                    
                    self.batch_log.append(f"✓ Konfiguration abgeschlossen")
                    success_count += 1
                    
                except serial.SerialException as e:
                    self.batch_log.append(f"✗ Serieller Fehler: {str(e)}")
                    fail_count += 1
                except Exception as e:
                    self.batch_log.append(f"✗ Fehler: {type(e).__name__} - {str(e)}")
                    fail_count += 1
                finally:
                    # Progress aktualisieren
                    self.batch_progress.setValue(int((i + 1) / total * 100))
                    QApplication.processEvents()
                
            self.batch_log.append(f"\n{'='*50}")
            self.batch_log.append("BATCH-KONFIGURATION ABGESCHLOSSEN")
            self.batch_log.append(f"{'='*50}")
            self.batch_log.append(f"Erfolgreich: {success_count}/{total}")
            self.batch_log.append(f"Fehlgeschlagen: {fail_count}/{total}")
            
            if success_count > 0:
                QMessageBox.information(self, "Batch-Konfiguration abgeschlossen", 
                    f"Erfolgreich konfiguriert: {success_count}/{total} Module\n"
                    f"Fehlgeschlagen: {fail_count}/{total} Module")
            else:
                QMessageBox.warning(self, "Batch-Konfiguration", 
                    "Keine Module konnten erfolgreich konfiguriert werden.\n"
                    "Überprüfen Sie die Verbindungen und Einstellungen.")
            
        except Exception as e:
            self.batch_log.append(f"\n✗ Kritischer Fehler: {type(e).__name__} - {str(e)}")
            QMessageBox.critical(self, "Kritischer Fehler", 
                f"Batch-Konfiguration abgebrochen:\n{str(e)}")
        finally:
            self.batch_config_btn.setEnabled(True)
            QApplication.processEvents()
            
    def closeEvent(self, event):
        """Cleanup beim Schließen mit robustem Error Handling"""
        try:
            # Stoppe Port-Timer
            if hasattr(self, 'port_timer') and self.port_timer is not None:
                try:
                    self.port_timer.stop()
                except Exception:
                    pass
            
            # Schließe Terminal-Verbindung
            if hasattr(self, 'terminal_connected') and self.terminal_connected:
                try:
                    if hasattr(self, 'terminal_timer') and self.terminal_timer is not None:
                        self.terminal_timer.stop()
                except Exception:
                    pass
                
                try:
                    if hasattr(self, 'terminal_serial') and self.terminal_serial is not None:
                        if self.terminal_serial.is_open:
                            self.terminal_serial.close()
                except Exception:
                    pass
            
            # Stoppe Worker-Threads
            if hasattr(self, 'current_worker') and self.current_worker is not None:
                try:
                    if self.current_worker.isRunning():
                        self.current_worker.stop()
                        self.current_worker.wait(1000)  # Warte max. 1 Sekunde
                        if self.current_worker.isRunning():
                            self.current_worker.terminate()
                except Exception:
                    pass
            
            # Schließe alle offenen seriellen Verbindungen
            if hasattr(self, 'serial_connections'):
                for port, conn in list(self.serial_connections.items()):
                    try:
                        if conn is not None and conn.is_open:
                            conn.close()
                    except Exception:
                        pass
                self.serial_connections.clear()
                        
        except Exception as e:
            # Auch bei Fehler beim Cleanup schließen wir die Anwendung
            print(f"Fehler beim Cleanup: {str(e)}")
        finally:
            event.accept()


def main():
    """Hauptfunktion mit umfassendem Error Handling"""
    # Setze Exception Hook für unbehandelte Exceptions
    def exception_hook(exctype, value, traceback_obj):
        """Handler für unbehandelte Exceptions"""
        import traceback
        
        # Formatiere Traceback
        tb_text = ''.join(traceback.format_exception(exctype, value, traceback_obj))
        
        # Logge in Datei (optional)
        try:
            with open('hc05_error.log', 'a', encoding='utf-8') as f:
                f.write(f"\n{'='*60}\n")
                f.write(f"Zeitstempel: {time.strftime('%Y-%m-%d %H:%M:%S')}\n")
                f.write(tb_text)
                f.write(f"{'='*60}\n")
        except Exception:
            pass  # Ignoriere Fehler beim Logging
        
        # Zeige Fehlermeldung
        try:
            error_msg = QMessageBox()
            error_msg.setIcon(QMessageBox.Critical)
            error_msg.setWindowTitle("Kritischer Fehler")
            error_msg.setText(f"Ein unerwarteter Fehler ist aufgetreten:\n\n{exctype.__name__}: {value}")
            error_msg.setDetailedText(tb_text)
            error_msg.setStandardButtons(QMessageBox.Ok)
            error_msg.exec()
        except Exception:
            # Fallback auf print wenn GUI nicht verfügbar
            print(f"KRITISCHER FEHLER:\n{tb_text}")
    
    # Setze Exception Hook
    sys.excepthook = exception_hook
    
    try:
        # Erstelle Qt-Anwendung
        try:
            app = QApplication(sys.argv)
        except RuntimeError as e:
            print(f"Fehler beim Erstellen der Qt-Anwendung: {str(e)}")
            print("Möglicherweise läuft bereits eine Instanz oder Qt ist nicht korrekt installiert.")
            sys.exit(1)
        
        # Setze Anwendungsmetadata
        try:
            app.setApplicationName("HC-05 Konfigurator")
            app.setOrganizationName("HC05Tools")
            app.setApplicationVersion("2.0")
        except Exception:
            pass  # Nicht kritisch
        
        # Setze Stil
        try:
            app.setStyle("Fusion")
            
            # Dunkles Theme
            palette = QPalette()
            palette.setColor(QPalette.Window, QColor(53, 53, 53))
            palette.setColor(QPalette.WindowText, Qt.white)
            palette.setColor(QPalette.Base, QColor(25, 25, 25))
            palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
            palette.setColor(QPalette.ToolTipBase, Qt.white)
            palette.setColor(QPalette.ToolTipText, Qt.white)
            palette.setColor(QPalette.Text, Qt.white)
            palette.setColor(QPalette.Button, QColor(53, 53, 53))
            palette.setColor(QPalette.ButtonText, Qt.white)
            palette.setColor(QPalette.BrightText, Qt.red)
            palette.setColor(QPalette.Link, QColor(42, 130, 218))
            palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
            palette.setColor(QPalette.HighlightedText, Qt.black)
            app.setPalette(palette)
        except Exception as e:
            print(f"Warnung: Stil konnte nicht gesetzt werden: {str(e)}")
            # Fortfahren ohne Stil
        
        # Erstelle Hauptfenster
        try:
            window = HC05Configurator()
            window.show()
        except Exception as e:
            QMessageBox.critical(
                None,
                "Fehler beim Starten",
                f"Das Hauptfenster konnte nicht erstellt werden:\n\n{type(e).__name__}: {str(e)}\n\n"
                "Bitte überprüfen Sie:\n"
                "- Sind PySide6 und pyserial installiert?\n"
                "- Haben Sie die erforderlichen Systemrechte?\n"
                "- Ist Ihr System kompatibel?"
            )
            sys.exit(1)
        
        # Starte Event Loop
        try:
            exit_code = app.exec()
            sys.exit(exit_code)
        except Exception as e:
            print(f"Fehler in der Hauptschleife: {str(e)}")
            sys.exit(1)
            
    except KeyboardInterrupt:
        print("\nAnwendung durch Benutzer beendet (Ctrl+C)")
        sys.exit(0)
    except Exception as e:
        print(f"Kritischer Fehler beim Start: {type(e).__name__}: {str(e)}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()