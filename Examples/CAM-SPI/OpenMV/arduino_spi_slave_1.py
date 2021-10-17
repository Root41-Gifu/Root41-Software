import pyb, ustruct, time

text = "Hello World!\n"
data = ustruct.pack("<bi%ds" % len(text), 85, len(text), text) # 85 is a sync char.

spi = pyb.SPI(2, pyb.SPI.SLAVE, polarity=0, phase=0)

# NSS callback.
def nss_callback(line):
    global spi, data
    try:
        spi.send(data, timeout=1000)
    except OSError as err:
        pass # Don't care about errors - so pass.
        # Note that there are 3 possible errors. A timeout error, a general purpose error, or
        # a busy error. The error codes are 116, 5, 16 respectively for "err.arg[0]".

# Configure NSS/CS in IRQ mode to send data when requested by the master.
pyb.ExtInt(pyb.Pin("P3"), pyb.ExtInt.IRQ_FALLING, pyb.Pin.PULL_UP, nss_callback)

while(True):
    time.sleep(1)
