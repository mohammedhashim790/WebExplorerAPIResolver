// WebExplorerAPIResolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include<queue>
#include <filesystem>


using namespace std;


//Understanding Operator Overloading
//class Point {
//
//public:
//    Point(int theX, int theY) :x(theX), y(theY)
//    {}
//    // Print the object
//    friend ostream& operator <<(ostream& outputStream, const Point& p);
//private:
//    int x;
//    int y;
//};
//
//ostream& operator <<(ostream& outputStream, const Point& p) {
//    int posX = p.x;
//    int posY = p.y;
//
//    outputStream << "x=" << posX << "," << "y=" << posY;
//    return outputStream;
//}


DWORD mydrives = 100;
char lpBuffer[100];

namespace fs = std::filesystem;


string listLogicalDrives();

void ListEntitiesDFS(string fromPath);
void ListEntitiesBFS(string fromPath);

string accumulate(vector<string> vector);
bool isValidDirectory(fs::directory_entry entry);


class File {
private:
    string name;
    fs::directory_entry entryPoint;


public:
    void setName(string fileName) {
        this->name = fileName;
    }

    string getName() {
        if (this->name.empty()) {
            return "null";
        }
        return this->name;
    }

    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }

    string toString() {
        return "{\"File\" : \"" + this->getName() + "\"}";
    }


    File() {

    }
    File(string name) {
        this->setName(name);
    }


    File(string name, fs::directory_entry entryPoint) {
        this->setName(name);
        this->setEntryPoint(entryPoint);
    }

};

class Folder{


/// <summary>
///     Example
/// Folder folder;
/// folder.setName("*");
/// folder.addFile(File("12-03"));
/// folder.addFolder(Folder("Hello"));

/// folder.addFolder(Folder("New Folder"));
/// folder.addFile(File("File1"));


/// cout << folder.toString() << endl;
/// </summary>

private:
    string name;
    fs::directory_entry entryPoint;

    vector<Folder> subFolders;
    vector<File> subFiles;


    string accumulateFolders() {
        string res;
        for (Folder folder : subFolders) {
            //res += "\"";
            res += folder.toString();;
            //res += "\",";
            res += ",";
        }
        //res += "}";
        if (res.empty()) {
            return "null";
        }
        res = "[" + res;
        res = string(res.begin(), res.end() - 1);
        res += "]";
        return res;
    }

    string accumulateFiles() {
        string res;
        for (File file : subFiles) {
            //res += "\"";
            res += file.toString();
            //res += "\",";
            res += ",";
        }
        if (res.empty()) {
            return "null";
        }
        res = "[" + res;
        res = string(res.begin(), res.end() - 1);
        res += "]";
        return res;
    }


    


public:
    void setName(string name) {
        this->name = name;
    }

    string getName() {
        return this->name;
    }

    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }



    string toString() {
        return "{\"Folder \" : \"" + this->name + "\", \"subFolders\" : " + this->accumulateFolders() + ",\"files\" : " + this->accumulateFiles() + "}";
    }

    void addFile(File fileName) {
        this->subFiles.push_back(fileName);
    }

    void addFolder(Folder folder) {
        this->subFolders.push_back(folder);
    }

    Folder() {

    }

    Folder(string name) {
        this->setName(name);
    }


    Folder(string name,fs::directory_entry entryPoint) {
        this->setName(name);
        this->setEntryPoint(entryPoint);
    }

};



int main()
{
    int now = time(NULL);



    const string dDrive("D:\\");
    const string cDrive("C:\\");

    ListEntitiesBFS(dDrive);


    cout << endl << "Time Taken to Execute DFS : " << (time(NULL) - now)<<"\n\n\n";
}


void ListEntitiesDFS(string fromPath) {

    for (const auto& entry : fs::directory_iterator(fromPath)){
        cout << entry.path() << endl;
        if (entry.is_directory()) {
            ListEntitiesDFS(entry.path().string());
        }
    }

    cout << "\nEnd of " << fromPath << endl;
}



void ListEntitiesBFS(string fromPath) {

    cout << "From Path " << fromPath << endl << endl;

    queue<Folder> que;
    int iter = 0;
    int files = 0;

    //que.push(Folder("*"));
    Folder rootFolder("*");


    for (const auto& entry : fs::directory_iterator(fromPath)) {
        /*cout << entry.path() << " \t"  << isValidDirectory(entry) << endl;*/
        if (isValidDirectory(entry)) {
            Folder folder(entry.path().filename().generic_string(), entry);
            rootFolder.addFolder(folder);
            /*que.push(folder);*/
        }
        printf("%d\b",iter++);
    }


    cout << rootFolder.toString();

    //vector<string> folderVector;
    //vector<string> filesVector;


    //


    //while (!que.empty()) {
    //    try {
    //        fs::directory_entry entryPoint = que.front();
    //        que.pop();
    //        //cout << entryPoint.path() << endl;
    //        if (entryPoint.is_directory()) {
    //            for (const auto& entry : fs::directory_iterator(entryPoint.path())) {
    //                //cout << entry.path() << endl;
    //                if (isValidDirectory(entry)) {
    //                    que.push(entry);
    //                    printf("%d\b\b\b\b\b\b\b\b\b\b\b\b\b\b", iter++);
    //                    folderVector.push_back(entry.path().generic_string());
    //                }
    //                else {
    //                    files++;
    //                    filesVector.push_back(entry.path().generic_string());
    //                }
    //                
    //            }
    //        }
    //    }
    //    catch (exception e) {
    //        /*cout << e.what();*/
    //    }
    //}


    //cout << "Total Folders : " << iter << " " << folderVector.size() << endl;
    //cout << "Total Files : " << files << " " << filesVector.size() << endl;

    //string folders = accumulate(folderVector);
    //cout << "Folders : " << folders;

    cout << "\nEnd of " << fromPath << endl;
}

string accumulate(vector<string> vect) {
    string res("[");

    for (const auto& str : vect) {
        res += "\"";
        res += str;
        res += "\",";
    }

    return string(res.begin(),res.end()-1) + "]";
}

bool isValidDirectory(fs::directory_entry entry) {
    return
        (
            entry.is_directory() && 
            !(entry.path().generic_string().find("System Volume Information") != string::npos ||
            entry.path().generic_string().find("$RECYCLE.BIN") != string::npos));
}



/// <summary>
/// Return List of Drive Characters
/// </summary>
/// <returns>string</returns>
string listLogicalDrives() {

    /// <summary>
    /// Returns the BIT representation denoting Alphabets from LSB A-Z
    /// </summary>
    /// <returns> Integer </returns>
    DWORD bit = GetLogicalDrives();

    char drive = 'a';

    int buffer = 1;

    string drives = "[";

    for (drive = 'a'; drive <= 'z'; drive++) {
        if ((bit & buffer) != 0) {
            drives += "\"";
            drives += drive;
            drives += "\",";
        }
        buffer = buffer << 1;
    }
    drives = string(drives.begin(), drives.end() - 1);
    drives += ']';
    return drives;

}