import sys
import os
import requests


IP = "127.0.0.1"
PORT = 0
while PORT < 1024:
    try:
        PORT = int(input("Enter a port number > 1024: "))
    except ValueError:
        pass


http_proxy = "http://" + IP + ":" + str(PORT)
proxyDict = {
    "http" : http_proxy
}

def proxy_request(url):
    return requests.get(url, proxies=proxyDict)


def basic_good_test():
    url = "http://zebroid.ida.liu.se/goodtest2.html"
    r = proxy_request(url)
    assert(r.status_code == 200)


def google_test():
    url = "http://www.google.com/search?q=Britney+Spears"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.google.com/search?q=SpOnGeBoB"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.google.com/search?q=paris+hilton"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.google.com/search?q=norrköping"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)


def wikipedia_test():
    url = "http://www.wikipedia.com/wiki/Britney_Spears"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.wikipedia.com/wiki/SpOnGeBoB"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.wikipedia.com/wiki/paris_hilton"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)
    url = "http://www.wikipedia.com/wiki/norrköping"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)


def bad_content():
    url = "http://zebroid.ida.liu.se/badtest1.html"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 2" in r.content)


def spongebob():
    url = "http://zebroid.ida.liu.se/SpongeBob.html"
    r = proxy_request(url)
    assert(b"Net Ninny Error Page 1" in r.content)


def post_test():
    url = "http://ocsp.pki.goog/gts1o1"
    r = requests.post(url, proxies=proxyDict, data={"key": "value"})
    assert(r.status_code == 200)


def http_old():
    """ Test that requirement 1 is fullfilled by doing a simple HTTP/1.0 GET request """
    response = os.popen("curl --http1.0 -x 127.0.0.1:1234 http://zebroid.ida.liu.se/goodtest2.html").read()
    assert("<html>" in response)


if __name__ == "__main__":
    basic_good_test()
    spongebob()
    google_test()
    wikipedia_test()
    bad_content()
    http_old()
    post_test()
    print("ALL TESTS PASSED!")
