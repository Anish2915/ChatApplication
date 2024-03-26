# ChatApplication

## 📖 Introduction

Our Chat Application (from scratch) provides a versatile platform for users to engage in real-time conversations, whether for personal, professional, or recreational purposes. With its client-server architecture, the application enables multiple users to connect to a central server, facilitating communication across various devices and networks. Whether you're collaborating on a project, catching up with friends, or hosting virtual events, our Chat Application offers a convenient and efficient way to stay connected.

## 📝 Table of Contents

1. [Overview](#overview)
2. [Technology Used](#technology-used)
3. [Features](#features)
4. [Getting Started](#getting-started)
5. [Usage and Functionalities](#usage-and-functionalities)
6. [Authors](#authors)
7. [Liscence](#liscence)

## 📝 Overview

Our Chat Application is powered by a combination of robust technologies. C++ forms the backbone, handling core programming logic and backend server implementation. To create a dynamic user interface, we rely on OpenGL with ImGui, offering users an engaging experience. Boost/Asio manages networking, enabling seamless connections between clients and servers for real-time communication. For efficient data handling, RapidJSON ensures swift serialization and deserialization of JSON-formatted messages. MySQL serves as our backend database system, storing user data and chat logs securely. Additionally, we leverage MongoDB Atlas for cloud-based storage of server IP addresses, ensuring reachability to our servers. Together, these technologies form a cohesive and reliable platform for smooth messaging experiences.

## 📡 Technology Used

- **C++:** Utilized for the core programming logic and backend server implementation.
- **OpenGL(ImGui):** Employed for creating the user interface, providing a rich and interactive experience for users.
- **Boost/Asio:** Used for networking to establish connections between clients and servers, facilitating real-time communication.
- **RapidJSON:** Employed for efficient serialization and deserialization of data in JSON format, ensuring fast and reliable message transmission.
- **MySQL:** Chosen as the backend database management system for storing user data, chat logs, and other relevant information.
- **MongoDB Atlas:** Utilized for cloud-based storage of server IP addresses, for reachability to our servers.

## ✨ Features

- **Real-time Messaging:** Experience instant messaging with minimal latency, allowing for smooth and uninterrupted communication.

### RealTime Messaging video
https://github.com/Anish2915/ChatApplication/assets/137883198/1f191076-18af-40d1-8537-d834e103a31f

- **Group Messaging System:** Facilitate seamless communication among multiple users simultaneously, fostering collaboration and enhancing team productivity within a shared environment.

### Group Messaging video
https://github.com/Anish2915/ChatApplication/assets/137883198/b51e805c-b726-4e4a-afbc-3c7ab45cd48b


- **Scalability:** Scale the application effortlessly by adding additional servers to accommodate growing user bases or increasing message traffic.
- **Administrative Controls:** Manage user accounts, permissions, and chat rooms effectively with administrative tools and controls.
- **Offline Messaging:** Send and receive messages even when users are offline, with automatic synchronization upon reconnection.


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
