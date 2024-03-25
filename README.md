# ChatApplication

## 📖 Introduction

Our Chat Application provides a versatile platform for users to engage in real-time conversations, whether for personal, professional, or recreational purposes. With its client-server architecture, the application enables multiple users to connect to a central server, facilitating communication across various devices and networks. Whether you're collaborating on a project, catching up with friends, or hosting virtual events, our Chat Application offers a convenient and efficient way to stay connected.

## 📝 Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Technology Used](#technology-used)
4. [Getting Started](#getting-started)
5. [Usage and Functionalities](#usage-and-functionalities)
6. [Authors](#authors)
7. [Liscence](#liscence)

## 📝 Overview

Welcome to our Chat Application, a robust and scalable solution for real-time messaging needs. This application allows users to connect with each other over a network, exchanging messages seamlessly. Built with C++ and utilizing modern technologies such as Boost::Asio for networking, OpenGL for the user interface, and RapidJSON for efficient data serialization, our Chat Application offers a reliable and feature-rich platform for communication.

## ✨ Features

- **Real-time Messaging:** Experience instant messaging with minimal latency, allowing for smooth and uninterrupted communication.
- **Cross-Platform Compatibility:** Access the Chat Application from desktop computers, laptops, or mobile devices, regardless of the operating system.
- **Scalability:** Scale the application effortlessly by adding additional servers to accommodate growing user bases or increasing message traffic.
- **Customizable User Interface:** Customize the user interface to suit your preferences, with support for themes, fonts, and layout configurations.
- **Secure Communication:** Ensure the privacy and security of your conversations with encrypted communication channels and robust authentication mechanisms.
- **Rich Media Support:** Share images, files, and multimedia content seamlessly within the chat interface, enhancing the messaging experience.
- **Administrative Controls:** Manage user accounts, permissions, and chat rooms effectively with administrative tools and controls.
- **Offline Messaging:** Send and receive messages even when users are offline, with automatic synchronization upon reconnection.

## 📡 Technology Used

- **C++:** Utilized for the core programming logic and backend server implementation.
- **OpenGL:** Employed for creating the user interface, providing a rich and interactive experience for users.
- **Boost/Asio:** Used for networking to establish connections between clients and servers, facilitating real-time communication.
- **RapidJSON:** Employed for efficient serialization and deserialization of data in JSON format, ensuring fast and reliable message transmission.
- **MySQL:** Chosen as the backend database management system for storing user data, chat logs, and other relevant information.
- **MongoDB Atlas:** Utilized for cloud-based storage of server IP addresses, enhancing flexibility and accessibility.

## 🦜 Getting Started

Follow these steps to install the Chat Application in your system

1. Clone the repository:
    ```bash
    git clone https://github.com/Anish2915/ChatApplication.git
    ```

2. The three included Premake build files are Build.lua, Core/Build-Core.lua, and App/Build-App.lua - you can edit these to customize your build configurations, edit the names of your projects and workspace/solution, etc.
3. Open the Scripts/ directory and run the appropriate Setup script to generate project files. You can edit the setup scripts to change the type of project that is generated - out of the box they are set to Visual Studio 2022 for Windows and gmake2 for Linux.
Note that no macOS setup script is currently provided; you can duplicate the Linux script and adjust it accordingly.

## 👨🏼‍💻 Usage and Functionalities

## 🎓 Author

<p> <a href="https://github.com/Anish2915"><b>Anish Kumar</b><a/></p>
<p> <a href="https://github.com/prakharmoses"><b>Prakhar Moses </b><a/></p>

## 📰 License
This Chat Application is not under any liscence. The viewer can't use or modify it without the written permission of the above authors.
