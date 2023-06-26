str = """Standard headers
A-IM
Accept
Accept-Charset
Accept-Encoding
Accept-Language
Accept-Datetime
Access-Control-Request-Method
Access-Control-Request-Headers
Authorization
Cache-Control
Connection
Content-Length
Content-Type
Cookie
Date
Expect
Forwarded
From
Host
If-Match
If-Modified-Since
If-None-Match
If-Range
If-Unmodified-Since
Max-Forwards
Origin
Pragma
Proxy-Authorization
Range
Referer
TE
User-Agent
Upgrade
Via
Warning
Non-standard headers
Dnt
X-Requested-With
X-CSRF-Token"""

tmp = str.split("\n")

for i in tmp:
    print(" valid_headers.push_back(\"", i,":\");" )