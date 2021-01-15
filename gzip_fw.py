Import("env") 


def gzip_firmware(source, target, env) -> None:
    print('gzip_firmware STARTED')

    if env['PIOPLATFORM'] != "espressif8266":
        print('GZipping not needed')
        return
    if env['UPLOAD_PROTOCOL'] != 'espota':
        print('Not an OTA upload, not GZipping')
        return
        
    print('ZIPINNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNG')
    import gzip
    import io
    import os

    scons_file = target[0]
    firmware_path = scons_file.__str__()
    gzip_path = firmware_path + '.gz'
    orig_path = firmware_path + '.orig'
    if os.path.exists(gzip_path):
        os.remove(gzip_path)
    print('GZipping firmware ' + firmware_path)
    with open(firmware_path, 'rb') as firmware_file, \
            gzip.open(gzip_path, 'wb') as dest:
        data = firmware_file.read()
        dest.write(data)
    orig_size = os.stat(firmware_path).st_size
    print("Original size {:d} 0x{:x}".format(orig_size, orig_size))
    gzip_size = os.stat(gzip_path).st_size
    print("Gzip size {:d} 0x{:x}, align {:d}".format(gzip_size, gzip_size, gzip_size % 4))

    if os.path.exists(orig_path):
        os.remove(orig_path)
    print('Moving original firmware to ' + orig_path)
    os.rename(firmware_path, orig_path)
    os.rename(gzip_path, firmware_path)
    print('Done')

env.AddPostAction("$BUILD_DIR/firmware.bin", gzip_firmware)