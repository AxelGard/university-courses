# Assignment 2 – Net Ninny. A Web Proxy Based Service
* Include a report for the assignment in this folder as a *PDF or txt* file.
* Include *source code* for your proxy implementation in this folder as well.
* Do not compress anything in this folder.


### check list

- [x] The proxy should support both HTTP/1.0 and HTTP/1.1. --> curl --http1.0 -x 127.0.0.1:1234 http://zebroid.ida.liu.se/goodtest2.html
- [x] Handles simple HTTP GET interactions between client and server
- [x] Blocks requests for undesirable URLs, using HTTP redirection to display this or this error page instead
- [x] Detects inappropriate content bytes within a Web page before it is returned to the user, and redirecting to this or this error page
- [x] Imposes no limit on the size of the transferred HTTP data
        Note: Using the realloc() function to increase the size of the buffer (allocated to receiving the HTTP data from the server) is also considered as imposing a limit. Instead, you must chose a size for the buffer and manage the data in it intelligently so that it does not result in lost packets or unsent data being overwritten when receiving responses. It is also recommended that you avoid performing type casting operations when receiving and sending data from the buffer.
- [x] Is compatible with all major browsers (e.g. Internet Explorer, Mozilla Firefox, Google Chrome, etc.) without the requirement to tweak any advanced feature
- [x] Allows the user to select the proxy port (i.e. the port number should not be hard coded)
- [x] Is smart in selection of what HTTP content should be searched for the forbidden keywords. For example,
      you probably agree that it is not wise to search inside compressed or other non-text-based HTTP content such as graphic files, etc.
- [ ] (Optional) Supporting file upload using the POST method
- [x] You do not have to relay HTTPS requests through the proxy


# Net Ninny proxy

## User manual

### Installing
Depending on your OS of choice some of this instructions might be different.
For this manual we will assume that the system is Linux Ubuntu.

start with downloading and  enter the repository
```bash
git clone git@gitlab.liu.se:axega544/networks.git
cd networks/
```

We have some requirements that is needed for the test
but the proxy server only needs std libraries
so you might want to have a virtual environment

On macOS and Linux:

```bash
python3 -m venv env
source env/bin/activate
pip3 install -r requirements.txt --user virtualenv
source deactivate # to exit out of the virtual environment
```


### Deploy
you are now ready to start your proxy server.
```bash
python3 server.py
```

the server will ask you for a port to enter.
This port need to be above 1024.
Enter the number and hit enter.

If the programs ask for the port again you have entered a non excepted value.


### Connecting to the proxy
You can now chose to use the testing script or
connect by your system or application of choice.

Depending on you choice of connection the set up process will differ.

Ex.
In Firefox you can change to proxy setting by clicking on the
cog wheel up in the top right corner of the home screen.
Then search for proxy and hit the settings... button that should appear.

Select the Manual proxy configuration and at HTTP Proxy enter the IP address
of the machine that is running the proxy
if the server is running on your own machine you can set 127.0.0.1
and then set the port number to set port when starting the proxy server

click on OK and you are all set to start using the proxy.
Be aware that you might need to clear cache
to see the affect on some common websites.


## Testing
to run the tests you need to have the
requirements installed on you machine or virtual environment

then you can just run the test script.
```bash
python3 testing.py
```

the script will ask you for a port to enter.
This port need to be the same as before.
Enter the number and hit enter.
