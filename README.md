# Apoapse Pro Server
The Apoapse server is part of the Apoapse Pro messaging platform offering a secure and decentralized collaborative space for communicating and sharing files.

[🚀 Find out more about Apoapse](https://apoapse.space/) | [📥 Receive all news and updates directly to your inbox](https://apoapse.space/newsletter-opt-in/)

## How to build
To compile the Apoapse Pro Server, you first need to compile the [Apoapse Protocol](https://github.com/apoapse/ApoapseProtocol) as a static library that must be linked into the ApoapseServerShared dynamic library.
As such, you first need to obtain and compile the Apoapse Protocol following the [instructions on the Apoapse Protocol repository](https://github.com/apoapse/ApoapseProtocol).
With most of the third party libraries shared between these two repositories, both repositories must be placed into a same folder, alongside each other.
You can then compile the server using CMake or Visual Studio solution provided.

## Server configuration
To define the TCP ports used, create a ServerConfig.ini file in the same folder as the server executable with the following content: ``` server_port = 3000 ```.
Note that Apoapse Pro uses two TCP ports, the second port being n+1 of the main port, used for the file stream connection.

## Server quick start
Before starting an Apapese Pro server for the first time, check or change the port defined in ServerConfig.ini and make sure it is properly forwarded. Note that Apoapse Pro uses two TCP ports, the second port being n+1 of the main port. For instance in case the port defined is 3000, both 3000 and 3001 TCP ports need to be forwarded.
Both connections are encrypted with TLSv1.3 using temporary Diffie-Hellman key exchange ensuring perfect forward secrecy. As such, various encryption keys need to be generated. You can either generate them yourself or they will be generated automatically with OpenSSL the first time the server is launched.
If you decide to generate them manually, the Apoapse Pro server requires a certificate chain file (server.crt), a private key (server.key) and a 2048 bits Diffie-Hellman key (dh2048.pem).

To start a Apoapse Pro Server, run the ServerConsole executable.
You will need to create a administrator account to start using Apoapse Pro, this can only be done using the [Apoapse Client](https://apoapse.space/downloads/).
