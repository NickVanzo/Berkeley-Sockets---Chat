# Berkeley Sockets - Chat

This project is a simple chat server built using Berkeley sockets in C++. The server receives messages from a client and broadcasts them to all connected clients, enabling real-time communication.

# Features

- Built using Berkeley sockets in C++;
- Uses TCP;
- Uses IPv4 addresses;
- Supports UNIX-based systems (Linux/macOS);
- Messages are exchanged as plain text;

# Limitations & Future Improvements

- Windows support: Currently, the project only works on UNIX-based systems. Adding Windows compatibility would be a great improvement;
- Endianness handling: The project does not support different endianness, but since it only exchanges strings, this is unlikely to be an issue;

<img width="1252" alt="Screenshot 2025-03-31 at 09 48 48" src="https://github.com/user-attachments/assets/cebbf5e3-84e0-4c1e-9e23-cc37b3699091" />
