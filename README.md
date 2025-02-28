# Email Client with IMAP Support (C)

This project is a simple email client written in C that downloads and parses emails from a standards-compliant IMAP server. It was developed as part of COMP30023 at the University of Melbourne.

## Project Overview

The primary goal of this project was to gain experience with socket programming and network protocols. The client implements core IMAP functionalities, including:

* Secure authentication
* Message retrieval
* Header parsing
* MIME decoding
* Listing emails in a mailbox

The project also incorporates security measures and adheres to relevant RFC standards.

## Features

* **IMAP Protocol Implementation:** Downloads and parses emails using the IMAP protocol (RFC 3501).
* **Secure Authentication:** Supports secure login to IMAP servers.
* **Message Retrieval:** Fetches raw email messages from the server.
* **Header Parsing:** Extracts and displays email headers (From, To, Date, Subject).
* **MIME Decoding:** Parses and decodes MIME-encoded emails (RFC 2045, RFC 2046) to display plain text content.
* **Email Listing:** Lists email subjects in a specified mailbox.
* **Error Handling:** Robust error handling for invalid commands, connection failures, and parsing errors.
* **Command-Line Interface:** User-friendly command-line interface with various options.

## Compilation

To compile the project, ensure you have a C compiler (e.g., GCC) installed. Then, simply run:

```bash
make
```
This will generate the fetchmail executable.

To clean the project, run:
```bash
make clean
```
 ## Usage

The program is executed using the following command-line arguments:
```bash
fetchmail -u <username> -p <password> [-f <folder>] [-n <messageNum>] <command> <server_name>
```
Where:

* -u <username>: IMAP username.
* -p <password>: IMAP password.
* -f <folder>: (Optional) Folder to access. Defaults to INBOX.
* -n <messageNum>: (Optional) Message sequence number. Fetches the last message if not specified.
* <command>: One of retrieve, parse, mime, or list.
* <server_name>: IMAP server hostname.

## Dependencies
C compiler (GCC or similar)

## Standards
This project adheres to the following RFC standards:
* RFC 3501: Internet Message Access Protocol Version 4rev1
* RFC 2045: Multipurpose Internet Mail Extensions (MIME) Part One: Format of Internet Message Bodies
* RFC 2046: Multipurpose Internet Mail Extensions (MIME) Part Two: Media Types1
