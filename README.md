# ServidorC

ServidorC is a server-side C program designed to provide a simple interface for clients to connect and interact with the server. The program is built using the BSD sockets API and supports multiple client connections. The program provides a command-line interface that allows users to execute commands and receive responses from the server.

This repository contains a testing version of the ServidorC program. The purpose of this testing version is to allow users to experiment with the program and provide feedback on any issues or bugs they encounter.

## Installation

To install the ServidorC program, you will need to clone the repository to your local machine. You can do this by running the following command in your terminal:

```bash
git clone https://github.com/Felamar/ServidorC.git
```

Once you have cloned the repository, you will need to navigate to the `ServidorC` directory and compile it manually. You can do this by running the following commands in your terminal:

```bash
~$ cd ServidorC/src
~$ gcc -o ../build/servidor servidor.c socketUtil.h
```

## Usage

This server simulates a ticket selling page for a movie theater. To start the server, run the following command in your terminal:
```bash
../build/servidor
```
Once the server is running, you can use a web browser to connect to it at http://localhost:5000. The server provides a simple web page that allows users to view available movies and purchase tickets for a specific showing.

On the main page, users can see a list of available movies. To purchase tickets, the user can click on a movie and select the desired showtime. The server will then present the user with a ticket confirmation page showing the details of the purchase.

The following routes are currently supported by the server:

    /: The main page that lists available movies and showtimes.

If you encounter any issues or bugs while using this ticket selling server, please report them by opening an issue on this repository.
