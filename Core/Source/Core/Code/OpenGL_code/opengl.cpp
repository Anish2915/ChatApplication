#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "opengl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../MySql_code/MySql.h"
#include "../Network_code/Network.h"
#include <algorithm>

std::vector<std::pair<std::string, int>> opengl::ConversationList;
std::vector<std::pair<std::string, std::string>> opengl::Messages;

std::string opengl::UserName = "default";
std::string opengl::MyIpAddress;
int opengl::UserId;
std::string opengl::ServerIpAddress;
std::string opengl::ChatName;

network::TCPClient* opengl::MyClientGui;

float opengl::WindowWidth = 800;
float opengl::WindowHeight = 600;
int opengl::COnvId;

void opengl::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WindowWidth = width;
    WindowHeight = height; 
}

void opengl::StartOpenGLWindowLoop(GLFWwindow* window)
{
    IMGUI_CHECKVERSION(); 
    ImGui::CreateContext(); 
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    
    bool CanShowMessages = false; 
    bool CanScroll = false;
    bool CanOpenDialogBox = false;
    bool CanOpenAddParticipant = false;
    bool CanOpenCreateGroup = false;
    bool someBool = false;

    ImFont* font1 = io.Fonts->AddFontFromFileTTF("arial.ttf", 26.0f); 
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("arial.ttf", 15.0f); 

    char text_buffer[1024] = "";
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); 

        ImGui_ImplOpenGL3_NewFrame();  
        ImGui_ImplGlfw_NewFrame();  
        ImGui::NewFrame();  

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        float c1w,c2w,c3w    ,ch; 
        if (WindowWidth < 1060) {
            c1w = 400;
            c2w = 500;
        }
        else if (WindowWidth>=1060 && WindowWidth<=1460) {
            c1w = 400 + (WindowWidth - 1060) / 4;
            c2w = (3 * WindowWidth)/4 - 295;
        }
        else {
            c1w = 500;
            c2w = 800 + (31 / 26) * (WindowWidth - 1460);
        }

        if (WindowHeight < 500) {
            ch = 500;
        }
        else {
            ch = WindowHeight;
        }

        
        ImGui::SetNextWindowSize(ImVec2(c1w, ch));
        ImGui::Begin((UserName +std::string("  ( UserId = ") + std::to_string(UserId) + " )" ).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        float buttonWidth = c1w-10;
        float buttonHeight = 30.0f;
        for (const auto& pair : ConversationList) {
            if (ImGui::Button(pair.first.c_str(), ImVec2(buttonWidth, buttonHeight))) {
                CanShowMessages = true;
                someBool = true;
                COnvId = pair.second;
                ChatName = (pair.first).c_str();
                CanScroll = true;
                std::vector<std::pair<std::string, std::string>> dataToSend;
                dataToSend.push_back(std::make_pair("Command", "1"));
                dataToSend.push_back(std::make_pair("ConversationId", std::to_string(pair.second)));
                opengl::MyClientGui->write_buffer_ = network::StrPairTOjson(dataToSend);
                opengl::MyClientGui->sendData(opengl::MyClientGui->write_buffer_);
            }
        }

        //bool show_another_window = true;

        ImGui::PushFont(font1);
        if (CanShowMessages) {
            ImGui::SetNextWindowPos(ImVec2(c1w, 0));  
            ImGui::SetNextWindowSize(ImVec2(c2w, ch-120));  
            ImGui::Begin(ChatName.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            int i = 0;
            for (const auto& pair2 : Messages) {

                int n;
                if (pair2.first == UserName) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));  // Make the child window background transparent
                    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
                    ImGui::SetCursorPosX(210);
                    ImVec2 size = ImGui::CalcTextSize((pair2.second).c_str());
                    n = size.x / (c2w - 220) + std::count(pair2.second.begin(), pair2.second.end(), '\n');
                    ImGui::PushFont(font2);
                    ImGui::BeginChild(++i, ImVec2(190, 32), true);
                    ImGui::Text(pair2.first.c_str());
                    ImGui::EndChild();
                    ImGui::PopFont();
                    ImGui::SetCursorPosX(210);
                    ImGui::BeginChild(++i, ImVec2(c2w - 220, ImGui::GetTextLineHeight() * (n + 2.5)), true);

                    ImGui::TextWrapped((pair2.second).c_str());

                    ImGui::EndChild();
                    ImGui::PopStyleColor(2);
                    continue;
                }
                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));  // Make the child window background transparent
                ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
                ImGui::SetCursorPosX(10);
                ImVec2 size = ImGui::CalcTextSize((pair2.second).c_str());
                n = size.x / (c2w - 220) + std::count(pair2.second.begin(), pair2.second.end(), '\n');
                ImGui::PushFont(font2);
                ImGui::BeginChild(++i, ImVec2(190, 32), true);
                ImGui::Text(pair2.first.c_str());
                ImGui::EndChild(); 
                ImGui::PopFont();
                ImGui::SetCursorPosX(10);
                ImGui::BeginChild(++i, ImVec2(c2w - 220, ImGui::GetTextLineHeight() * (n + 2.5)), true);
                ImGui::TextWrapped((pair2.second).c_str());

                ImGui::EndChild();
                ImGui::PopStyleColor(2);
            }

            if (CanScroll) {
                ImGui::SetScrollHereY(1.0f);
                CanScroll = false;
            }
            ImGui::End();
        }
        ImGui::PopFont();
        ImGui::End(); 

        ImGui::SetNextWindowPos(ImVec2(c1w + c2w, 10)); 
        ImGui::SetNextWindowSize(ImVec2(160, 120)); 
        ImGui::Begin("##,", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        
        if (ImGui::Button("New Chat", ImVec2(150, 70))) { 
            CanOpenDialogBox = true;
            CanOpenCreateGroup = false;
            CanOpenAddParticipant = false;
        }
        if (CanOpenDialogBox) {
            ImGui::SetNextWindowPos(ImVec2((WindowWidth / 2)-100, (WindowHeight / 2)-100));
            ImGui::SetNextWindowSize(ImVec2(200, 170));

            ImGui::Begin("Enter User Id", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            static char text[4] = "";
            ImGui::InputText("##text1", text, IM_ARRAYSIZE(text),ImGuiInputTextFlags_CharsDecimal);

            if (ImGui::Button("OK", ImVec2(90, 80))) {
                if (text[0] != '\0') {
                    std::vector<std::pair<std::string, std::string>> temp;
                    temp.push_back({ "Command","3" });
                    temp.push_back({ "CreaterId",std::to_string(opengl::UserId) });
                    temp.push_back({ "AdderId",text });
                    opengl::MyClientGui->write_buffer_ = network::StrPairTOjson(temp);
                    opengl::MyClientGui->sendData(opengl::MyClientGui->write_buffer_);
                    CanOpenDialogBox = false;   
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(90, 80))) {
                CanOpenDialogBox = false;
            }

            ImGui::End();
        }
        ImGui::End();

        
        ImGui::SetNextWindowPos(ImVec2(c1w + c2w, 140)); 
        ImGui::SetNextWindowSize(ImVec2(160, 120)); 
        ImGui::Begin("##'", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Create Group", ImVec2(150, 70))) { 
            CanOpenCreateGroup = true;
            CanOpenDialogBox = false;
            CanOpenAddParticipant = false;
        }

        if (CanOpenCreateGroup) {
            ImGui::SetNextWindowPos(ImVec2((WindowWidth / 2) - 100, (WindowHeight / 2) - 100));
            ImGui::SetNextWindowSize(ImVec2(200, 170));

            ImGui::Begin("Enter Group Name", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            static char text2[12] = "";
            ImGui::InputText("##text2", text2, IM_ARRAYSIZE(text2));

            if (ImGui::Button("OK", ImVec2(90, 80))) { 
                if (text2[0] != '\0') {
                    std::vector<std::pair<std::string, std::string>> temp;
                    temp.push_back({ "Command","4" });
                    temp.push_back({ "CreaterId",std::to_string(opengl::UserId) });
                    temp.push_back({ "GroupName",text2 });
                    opengl::MyClientGui->write_buffer_ = network::StrPairTOjson(temp);
                    opengl::MyClientGui->sendData(opengl::MyClientGui->write_buffer_);
                    CanOpenCreateGroup = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(90, 80))) {
                CanOpenCreateGroup = false; 
            }

            ImGui::End();
        }

        ImGui::End();
        if (someBool) { 
            ImGui::SetNextWindowPos(ImVec2(c1w + c2w, 270)); 
            ImGui::SetNextWindowSize(ImVec2(160, 120));
            ImGui::Begin("##;", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        
            if (ImGui::Button("    Add \nParticipant", ImVec2(150, 70))) {
                CanOpenAddParticipant = true;
                CanOpenCreateGroup = false;
                CanOpenDialogBox = false;
            }
            ImGui::End();
        }

        if (CanOpenAddParticipant) {
            ImGui::SetNextWindowPos(ImVec2((WindowWidth / 2) - 100, (WindowHeight / 2) - 100));
            ImGui::SetNextWindowSize(ImVec2(200, 170));

            ImGui::Begin("Enter His Id", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            static char text3[4] = "";
            ImGui::InputText("##text3", text3, IM_ARRAYSIZE(text3), ImGuiInputTextFlags_CharsDecimal);

            if (ImGui::Button("OK", ImVec2(90, 80))) {
                if (text3[0] != '\0') {
                    std::vector<std::pair<std::string, std::string>> temp;
                    temp.push_back({ "Command","5" });
                    temp.push_back({ "ConversationId",std::to_string(opengl::COnvId) });
                    temp.push_back({ "UserId",text3 });
                    opengl::MyClientGui->write_buffer_ = network::StrPairTOjson(temp);  
                    opengl::MyClientGui->sendData(opengl::MyClientGui->write_buffer_); 
                    CanOpenAddParticipant = false;
                }
            }
            ImGui::SameLine(); 
            if (ImGui::Button("Cancel", ImVec2(90, 80))) { 
                CanOpenAddParticipant = false;
            }

            ImGui::End();  
        }


        if (CanShowMessages) {
            ImGui::SetNextWindowPos(ImVec2(c1w, ch - 120));
            ImGui::SetNextWindowSize(ImVec2(c2w, 120));
            ImGui::Begin("Write Message", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            ImGui::InputTextMultiline("##Text", text_buffer, IM_ARRAYSIZE(text_buffer), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 2.8), ImGuiInputTextFlags_AllowTabInput);
            ImGui::End();


            ImGui::SetNextWindowPos(ImVec2(c1w + c2w, ch - 120));
            ImGui::SetNextWindowSize(ImVec2(160, 120));
            ImGui::Begin("##.", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
            if (ImGui::Button("Send", ImVec2(150, 70))) {

                std::vector<std::pair<std::string, std::string>> msgToSend;
                msgToSend.push_back({ "Command","2" });
                msgToSend.push_back({ "SenderID", std::to_string(UserId) });
                msgToSend.push_back({ "ConversationID",std::to_string(COnvId) });
                msgToSend.push_back({ "Messages", text_buffer });
                std::cout << text_buffer << std::endl;
                MyClientGui->write_buffer_ = network::StrPairTOjson(msgToSend);
                MyClientGui->sendData(MyClientGui->write_buffer_);
                memset(text_buffer, 0, sizeof(text_buffer));

            }
            ImGui::End();
        }

        ImGui::Render(); 
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        glfwSwapBuffers(window); 
        glfwWaitEvents(); 
    } 
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplGlfw_Shutdown(); 
    ImGui::DestroyContext(); 
    glfwTerminate(); 
}

void opengl::SetCallback(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, &opengl::framebuffer_size_callback); 
}

void opengl::SetUserId(int a)
{
    UserId = a;
}



void opengl::CreateWindowClient()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "ChatApplication", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    glewInit();
    if (glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, WindowWidth, WindowHeight);


    SetCallback(window);
    StartOpenGLWindowLoop(window);
}
