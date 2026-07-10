# EasyHTTPServer
Light-weight http server


This project demonstrates the fundamentals of TCP sockets, HTTP request parsing, multithreaded connecting handling, static file serving, and HTTP response generation.

## Features
- Serves static files using HTTP
- Supports HTTP GET requests
- Automatically serves index.html for directory paths
- Returns 404 responses when requested files are not found
- Detects MIME types using file extensions
- Works on both Windows and Linux platforms

Requirements
- A compiler with C++20 support
- Windows or Linux


## Usage
Place the files you want to serve inside the html/ directory.

Example:

html/ 
├── index.html 
├── styles.css 
├── script.js 
└── images/ 
    └── logo.png

Start the server and open the following address in a browser: http://localhost

A request for: 
    http://localhost/ 
will serve: html/index.html

A request for:
    http://localhost/images/logo.png
will serve: html/images/logo.png

## Current Limitations
- The current implementation:
- Supports only GET requests
- Does not support HTTPS or TLS
- Does not support request bodies
- Does not support chunked transfer encoding
- Does not support persistent connections
- Does not fully validate all HTTP headers
- Loads an entire requested file into memory before sending it
- Does not currently implement caching or range requests
- Has not been hardened against every malformed or malicious request
- Is not intended for production hosting



## Background
This server was originally created in 2018 as part of a larger application. Its original purpose was to expose a lightweight browser-based control panel that could trigger commands through HTTP GET requests.

The project has since been separated and revised as a standalone demonstration of low-level networking and HTTP server development in C++.
