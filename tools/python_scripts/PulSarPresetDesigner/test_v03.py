import json
import tempfile
import threading
import http.server
from pathlib import Path
from palette_catalog import read_source_palettes, device_palettes, palette_names_from_library
from main import Catalog, http_get_presets, http_get_presets_bytes, http_upload_presets

with tempfile.TemporaryDirectory() as tmp:
    h=Path(tmp)/'mPalette_Progmem.h'
    h.write_text('#define D_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__NAME_CTR "Snowy 02/Snow Tree Purple"\n#define D_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__NAME_CTR "Colour and White Stripe"\n')
    result=read_source_palettes([h])
    assert 'Snowy 02' in result and 'Snow Tree Purple' in result and 'Colour and White Stripe' in result
assert device_palettes({'0':'Snowy 02','1':'Red'}) == {'0':'Snowy 02','1':'Red'}
assert palette_names_from_library(Catalog())
class Handler(http.server.BaseHTTPRequestHandler):
    value=b'{ "0":{}, "1":{"n":"Original 1"} }\n'
    def do_GET(self):
        if self.path!='/presets.json': self.send_error(404);return
        self.send_response(200);self.end_headers();self.wfile.write(self.value)
    def do_POST(self):
        if self.path!='/upload': self.send_error(404);return
        raw=self.rfile.read(int(self.headers['Content-Length']))
        Handler.value=raw.split(b'\r\n\r\n',1)[1].rsplit(b'\r\n--',1)[0]
        self.send_response(200);self.end_headers();self.wfile.write(b'File Uploaded!')
    def log_message(self,*args):pass
server=http.server.HTTPServer(('127.0.0.1',0),Handler)
thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start()
try:
    port=server.server_port
    previous=http_get_presets_bytes('127.0.0.1',port)
    assert previous==Handler.value
    upload={'0':{},'1':{'n':'Updated'}}
    result=http_upload_presets('127.0.0.1',port,upload)
    assert result[0]==200
    assert http_get_presets('127.0.0.1',port)==upload
finally:
    server.shutdown();server.server_close()
print('v0.3 palette parsing, library palette names, HTTP download and multipart upload: OK')
