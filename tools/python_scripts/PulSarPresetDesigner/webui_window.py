"""Launch a full PulSar WebUI with a JavaScript-capable desktop browser window.
Run as a separate process so pywebview owns its UI main thread.
"""
import sys
if len(sys.argv)!=2 or not sys.argv[1].startswith(('http://','https://')):
    raise SystemExit('Usage: python webui_window.py http://DEVICE_IP/')
try:
    import webview
except ImportError as exc:
    raise SystemExit('Install pywebview: python -m pip install pywebview') from exc
webview.create_window('PulSar — Device WebUI',sys.argv[1],width=1300,height=900)
webview.start()
