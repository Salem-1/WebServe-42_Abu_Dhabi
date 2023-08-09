import http.client
import time

chunk1 = "This is the first chunk"
chunk2 = "This is the second chunk"

conn = http.client.HTTPConnection('localhost:5555')
conn.connect()
conn.putrequest('POST', '/')
conn.putheader('Transfer-Encoding', 'chunked')
conn.putheader('Content-Type', 'application/x-www-form-urlencoded')
conn.endheaders()

conn.send("%s\r\n" % hex(len(chunk1))[2:])
conn.send("%s\r\n" % chunk1)

time.sleep(1)

conn.send("%s\r\n" % hex(len(chunk2))[2:])
conn.send("%s\r\n" % chunk2)

time.sleep(1)
# last chunk
conn.send("0\r\n\r\n")

r = conn.getresponse()
print(r.status, r.reason, r.read())
