#!/usr/bin/python3.5
# A simple Python-based web server example.
# In client side, you can use following shell command to send a request to this server:
# curl "http://127.0.0.1:8080/?image_path=/root/mobike.jpg"

from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qsl

class MyRequestHandler(BaseHTTPRequestHandler):
  def do_GET(self):
    # e.g. "/?image_path=/root/mobike.jpg"
    path = self.path
    # e.g. "/root/mobike.jpg"
    image_path = parse_qsl(path[2:])[0][1] + '\n'

    # send response status code
    self.send_response(200)
    
    # send headers
    self.send_header('Content-type','text/html')
    self.end_headers()
    
    # send message back to client, write content as utf-8 data
    self.wfile.write(bytes(image_path, "utf8"))
    return
    
def start_web_server():
  print('Starting web server...')
    
  server_address = ('127.0.0.1', 8080)
  httpd = HTTPServer(server_address, MyRequestHandler)
  httpd.serve_forever()


start_web_server()
