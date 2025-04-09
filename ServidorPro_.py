import socket
import threading
import logging
from typing import Optional

logging.basicConfig(level=logging.INFO, format='[%(levelname)s] %(message)s')

class ClientType:
    SENSOR = "SENSOR"
    LED_CONTROLLER = "LED_CONTROLLER"

class SensorServer:
    INTERVALS = [
        (0, 10),
        (10, 20),
        (20, 30),
        (0, 0)  # Default/error
    ]

    def __init__(self, host: str = '0.0.0.0', port: int = 12345):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        self.sensor_client: Optional[socket.socket] = None
        self.led_client: Optional[socket.socket] = None
        self.current_interval: int = -1
        self.lock = threading.Lock()

    def start(self):
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        logging.info(f"Server listening on {self.host}:{self.port}")

        try:
            while True:
                client_socket, addr = self.server_socket.accept()
                logging.info(f"Connection from {addr}")
                threading.Thread(target=self.handle_new_connection, args=(client_socket,), daemon=True).start()
        except KeyboardInterrupt:
            logging.info("Server shutting down...")
        finally:
            self.server_socket.close()

    def handle_new_connection(self, client_socket: socket.socket):
        try:
            client_type = client_socket.recv(1024).decode().strip()

            if client_type == ClientType.SENSOR:
                self.replace_client('sensor', client_socket)
                threading.Thread(target=self.handle_sensor, args=(client_socket,), daemon=True).start()
            elif client_type == ClientType.LED_CONTROLLER:
                self.replace_client('led', client_socket)
                threading.Thread(target=self.handle_led_controller, args=(client_socket,), daemon=True).start()
            else:
                logging.warning("Unknown client type. Closing connection.")
                client_socket.close()
        except Exception as e:
            logging.error(f"Error handling new connection: {e}")
            client_socket.close()

    def replace_client(self, client_role: str, new_socket: socket.socket):
        with self.lock:
            if client_role == 'sensor':
                if self.sensor_client:
                    logging.info("Replacing existing sensor connection")
                    self.sensor_client.close()
                self.sensor_client = new_socket
            elif client_role == 'led':
                if self.led_client:
                    logging.info("Replacing existing LED controller connection")
                    self.led_client.close()
                self.led_client = new_socket

    def handle_sensor(self, client_socket: socket.socket):
        try:
            while True:
                data = client_socket.recv(1024).decode().strip()
                if not data:
                    break
                logging.info(f"Sensor says: {data}")

                if data.startswith("PUT"):
                    self.process_put(data)
                elif data == "GET_INTERVALS":
                    self.send_intervals(client_socket)
        except (ConnectionResetError, OSError):
            logging.warning("Sensor disconnected unexpectedly")
        finally:
            self.cleanup_client(client_socket, 'sensor')

    def handle_led_controller(self, client_socket: socket.socket):
        try:
            while True:
                data = client_socket.recv(1024)
                if not data:
                    continue
                message = data.decode().strip()
                logging.info(f"LED Controller says: {message}")
        except (ConnectionResetError, OSError):
            logging.warning("LED controller disconnected unexpectedly")
        finally:
            self.cleanup_client(client_socket, 'led')

    def process_put(self, data: str):
        parts = data.split()
        if len(parts) < 2 or not parts[1].isdigit():
            logging.warning("Invalid PUT command")
            return

        new_interval = int(parts[1])
        with self.lock:
            if new_interval != self.current_interval:
                self.current_interval = new_interval
                logging.info(f"Interval changed to: {new_interval}")
                self.update_leds()

    def send_intervals(self, client_socket: socket.socket):
        intervals_str = "\n".join(f"{i},{start},{end}" for i, (start, end) in enumerate(self.INTERVALS))
        client_socket.sendall(intervals_str.encode())
        logging.info("Sent intervals to sensor")

    def update_leds(self):
        if not self.led_client:
            logging.warning("No LED controller connected")
            return
        command = self.generate_led_command()
        try:
            self.led_client.sendall(command.encode())
            logging.info(f"Sent to LED controller: {command}")
        except Exception as e:
            logging.error(f"Failed to send LED command: {e}")

    def generate_led_command(self) -> str:
        mapping = {
            0: "POST rLed:ON yLed:OFF gLed:OFF",
            1: "POST rLed:OFF yLed:ON gLed:OFF",
            2: "POST rLed:OFF yLed:OFF gLed:ON",
            3: "POST rLed:ON yLed:ON gLed:ON"

        }
        return mapping.get(self.current_interval, "POST rLed:ON yLed:OFF gLed:OFF")

    def cleanup_client(self, client_socket: socket.socket, role: str):
        client_socket.close()
        with self.lock:
            if role == 'sensor' and self.sensor_client == client_socket:
                self.sensor_client = None
            elif role == 'led' and self.led_client == client_socket:
                self.led_client = None
        logging.info(f"{role.capitalize()} connection closed")

if __name__ == "__main__":
    server = SensorServer()
    server.start()
