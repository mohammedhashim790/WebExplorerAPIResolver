// WebExplorerAPIResolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include<queue>
#include <filesystem>
#include<string>
#include<jni.h>
#include "Resolver.h"
#include<stack>


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



/// <summary>
/// Set Seek for Generation 
/// </summary>
/// <param name=""></param>




DWORD mydrives = 100;
char lpBuffer[100];

namespace fs = std::filesystem;




string decToHexa(int n)
{
    // ans string to store hexadecimal number
    string ans = "";

    while (n != 0) {
        int rem = 0;
        char ch;
        rem = n % 16;

        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55;
        }

        ans += ch;
        n = n / 16;
    }

    int i = 0, j = ans.size() - 1;

    return string(ans.rbegin(), ans.rend());
}


string RandomSeries() {
    //Set Seed

    /*srand(time(NULL));

    int sec = rand();


    srand(sec);*/


    int limit = 255;
    string series;
    for (int i = 0; i < 16; i++) {
        int num = 16 + (rand() % limit);
        series += decToHexa(num);
    }
    /// <summary>
    /// Filter to 32 bits
    /// </summary>
    /// <returns></returns>
    if (series.length() > 32) {
        series = series.substr(0, 32);
    }
    return series;
}

string UUIDV4() {

    return RandomSeries();
}





class File {
private:
    string Id;
    string name;
    fs::directory_entry entryPoint;
    size_t fileSize;
    string parentId;



public:
    void setName(string fileName) {
        this->name = fileName;
    }

    string getName() {
        if (this->name.empty()) {
            return "null";
        }
        return "\"" + this->name + "\""; 
    }

    void setFileSize(size_t fileSize) {
        this->fileSize = fileSize;
    }

    size_t getFileSize() {
        return fileSize;
    }

    string getParentId() {
        if (this->parentId.empty()) {
            return "null";
        }
        return "\"" + this->parentId + "\"";
    }



    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }

    string toString() {
        return "{\"Id\":\"" + this->Id + "\",\"Name\" : " + this->getName()+",\"Path\" : \"" + getEntryPoint().path().generic_string() + "\", \"Size\":" + to_string(entryPoint.file_size()) + ",\"parentFolderId\" : " + this->getParentId() + "}";
    }

    string toSimpleString() {
        return "{\"Id\":\"" + this->Id + "\",\"Name\" : \"" + this->name + "\"" + ",\"parentFolderId\" : " + this->getParentId() + ",\"Size\":" + to_string(entryPoint.file_size()) + "}";
    }


    File() {
        this->Id = UUIDV4();
    }
    File(string name) {
        this->Id = UUIDV4();
        this->setName(name);
    }


    File(string name, fs::directory_entry entryPoint,string parentId) {
        this->Id = UUIDV4();
        this->setName(name);
        this->setEntryPoint(entryPoint);
        this->setFileSize(entryPoint.file_size());
        this->parentId = parentId;
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
    string Id;
    string name;
    fs::directory_entry entryPoint;
    string parentId;

    

    vector<Folder*> subFolders;

    vector<File*> subFiles;
    //vector<File*> subFiles;


    string accumulateFolders() {
        string res;
        for (Folder* folder : subFolders) {
            //res += "\"";
            res += folder->toString();;
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
        for (File* file : subFiles) {
            //res += "\"";
            res += file->toString();
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
    string getParentId() {
        if (this->parentId.empty()) {
            return "null";
        }
        return "\"" + this->parentId + "\"";
    }
    string getId() {
        return this->Id;
    }


    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }



    string toString() {
        return "{\"Id\":\"" + this->Id + "\",\"Name\" : \"" + this->name  + "\",\"Path\" : \"" + getEntryPoint().path().generic_string() + "\"," + "\"subFolders\" : " + this->accumulateFolders() + ",\"files\" : " + this->accumulateFiles() + ",\"parentId\" : " + this->getParentId() + "}";
    }

    string toSimpleString() {
        return "{\"Id\":\"" + this->Id + "\",\"Name\" : \"" + this->name  + "\",\"Path\" : \"" + getEntryPoint().path().generic_string() +  "\"," + "\"parentId\" : " + this->getParentId() + "}";
    }



    void addFile(File* fileName) {
        this->subFiles.push_back(fileName);
    }

    void addFolder(Folder* folder) {
        this->subFolders.push_back(folder);
    }

    Folder() {
        this->Id = UUIDV4();
    }

    Folder(string name) {
        this->Id = UUIDV4();
        this->setName(name);
    }


    Folder(string name,fs::directory_entry entryPoint,string parentId) {
        this->setName(name);
        this->Id = UUIDV4();
        this->setEntryPoint(entryPoint);
        this->parentId = parentId;
    }

};


queue<Folder*> folders;
queue<File*> files;
string listLogicalDrives();

void ListEntitiesDFS(string fromPath);
string ListEntitiesBFS(string fromPath, JNIEnv* env);
void SampleRecursive(string fromPath);
string EntitiesFromPath(string fromPath, JNIEnv* env);

string accumulate(vector<string> vector);
bool isValidDirectory(fs::directory_entry entry);

void CallJava();




JNIEXPORT jstring JNICALL Java_com_example_webexplorerapi_Resolver_Resolver_ListDrive(JNIEnv* env, jobject thisObj) {

    return env->NewStringUTF(listLogicalDrives().c_str());
}


int ONE_TIME_LIMIT = 10000;

JNIEXPORT jlong JNICALL Java_com_example_webexplorerapi_Resolver_Resolver_ListFolder(JNIEnv* env, jobject thisObj, jstring fromPath) {


    string path(env->GetStringUTFChars(fromPath, 0));



    /// <summary>
    /// BFS
    /// </summary>
    /// <param name="env"></param>
    /// <param name="thisObj"></param>
    /// <param name="fromPath"></param>
    /// <returns></returns>
    /// 


    EntitiesFromPath(path,env).c_str();

    return folders.size();

}


Folder* rootFolder;


string makeFolder() {
    string json("\"Folders\" : [");

    int iter = 0;

    while (!folders.empty()) {
        auto var = folders.front();
        folders.pop();
        string res = var->toSimpleString();

        res += ",";
        json += res;
        iter++;
    }
    if (iter > 1)
        json = string(json.begin(), json.end() - 1);
    json += "],";
    return json;
}

string makeFile() {
    string json("\"Files\" : [");

    int iter = 0;

    while (!files.empty()) {
        auto var = files.front();
        files.pop();
        string res = var->toSimpleString();

        res += ",";
        json += res;
        iter++;
    }
    if (iter > 1)
        json = string(json.begin(), json.end() - 1);
    json += "]";
    return json;
}


JNIEXPORT jbyteArray JNICALL Java_com_example_webexplorerapi_Resolver_Resolver_Next(JNIEnv* env, jobject thisObj, jstring parentId) {

    
    string json("{");

    json += makeFolder();

    json += makeFile();
    json += "}";

    const char* res = json.c_str();

    cout << "Sending Length : \n" << strlen(res) << endl;

    jbyteArray arr = env->NewByteArray(json.length());
    env->SetByteArrayRegion(arr, 0, json.length(), (jbyte*)json.c_str());
    return arr;
}


jclass loadClass(JNIEnv* env) {
    jclass clas = env->FindClass("com/example/webexplorerapi/Controller/Queries");

    return clas;
}

void CheckClass(JNIEnv* env) {
    jclass cls = env->FindClass("com/example/webexplorerapi/Controller/Queries");

    cout << " EMpty Folder : " << (cls == nullptr);


    if (cls == nullptr) {
        cerr << "ERROR: class not found !";
    }
    else {                                  

        jmethodID mid = env->GetStaticMethodID(cls, "mymain", "()V");  // find method
        if (mid == nullptr)
            cerr << "ERROR: method void mymain() not found !" << endl;
        else {
            env->CallStaticVoidMethod(cls, mid);                      // call method
            cout << endl;
        }
    }

}


JNIEXPORT void JNICALL Java_com_example_webexplorerapi_Resolver_Resolver_TestEnv(JNIEnv* env, jobject thisObj) {

    CheckClass(env);

}






int main()
{
    int now = time(NULL);



    const string dDrive("D:\\");
    const string cDrive("C:\\");


    CallJava();


    //const string shortPath("D:\\Zoho\\Notes");

    //EntitiesFromPath(cDrive);

    /*string json("{");

    json += makeFolder();

    json += makeFile();
    json += "}";

    cout << json;*/


    //cout << " Char Length : " << strlen(json.c_str());

    cout << "\n\n\n\n\nTime Taken to Execute DFS : " << (time(NULL) - now)<<"\n\n\n";
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


/// <summary>
/// Similar to @ListEntitiesBFS()
/// </summary>
/// <param name="fromPath"></param>
void SampleRecursive(string fromPath) {

    for (const auto& entry : fs::recursive_directory_iterator(fromPath)){
        cout << entry.path() << endl;
    }

    cout << "\nEnd of " << fromPath << endl;
}



string EntitiesFromPath(string fromPath, JNIEnv* env) {
    srand(time(NULL));
    return ListEntitiesBFS(fromPath,env);
}



void SendToJava(JNIEnv* env,jclass jcls) {

    cout << "Sending to Java : " << endl;

    string json("{");

    json += makeFolder();

    json += makeFile();
    json += "}";

    //jmethodID mid = env->GetStaticMethodID(jcls, "GetEntityFromResolver", "(Ljava/lang/String;)V");  // find method
    jmethodID mid = env->GetStaticMethodID(jcls, "GetEntityFromResolver", "([B)V");  // find method
    if (mid == nullptr)
        cerr << "ERROR: method void mymain() not found !" << endl;
    else {
        jbyteArray byteArray = env->NewByteArray(json.size());
        env->SetByteArrayRegion(byteArray, 0, json.size(), (jbyte*)json.data());
        //env->CallStaticVoidMethod(jcls, mid, env->NewStringUTF(json.c_str()));                      // call method
        env->CallStaticVoidMethod(jcls, mid, byteArray);                      // call method
        cout << endl;
    }
}

string ListEntitiesBFS(string fromPath, JNIEnv* env) {

    cout << "From Path " << fromPath << endl << endl;


    jclass jcls = loadClass(env);

    queue<Folder*> que;
    int iter = 0;
    int filesIter = 0;
    int folderIter = 0;

    fs::directory_entry rootEntry = fs::directory_entry(fromPath);
    rootFolder =  new Folder(rootEntry.path().generic_string(),rootEntry,"");
    que.push(rootFolder);
    folders.push(rootFolder);

    //vector<string> folderVector;
    //vector<string> filesVector;


    while (!que.empty()) {
        try {
            Folder *parent = que.front();

            Folder *pRef = parent;

            //cout << "Reading From : " << parent->toString() << endl;

            que.pop();
            
            fs::path path = pRef->getEntryPoint().path();


            for (const auto& entry : fs::directory_iterator(path, fs::directory_options::skip_permission_denied)) {
                if (isValidDirectory(entry)) {
                    Folder *folder = new Folder(entry.path().filename().generic_string(), entry,pRef->getId());
                    que.push(folder);
                    parent->addFolder(folder);
                    folders.push(folder);
                    folderIter++;
                }
                else {
                    File *file = new File(entry.path().filename().generic_string(), entry, pRef->getId());
                    parent->addFile(file);
                    files.push(file);
                    filesIter++;
                }
                printf("%d\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", iter++);
            }

            if (folders.size() > ONE_TIME_LIMIT || files.size() > ONE_TIME_LIMIT) {
                SendToJava(env,jcls);
            }

        }
        catch (exception e) {
            //cout << "\n\n\nError  : " << e.what() << "\n\n\n";
            cout << e.what();
        }
    }


    cout << "\n\n\n";

    cout << "Total Folders : " << folderIter << " " << folders.size() << endl;
    cout << "Total Files : " << filesIter << " " << files.size() << endl;


    SendToJava(env,jcls);


    /*while (!folders.empty() || !files.empty()) {
        SendToJava(env, jcls);
    }*/



    cout << "\nEnd of " << fromPath << endl;

    cout << "Stack is Empty  : Folders : " << folders.size() << " Files : " << files.size() << endl;







    return rootFolder->toSimpleString();
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

void CallJava() {


    //string /*classpath = "D:\\Zoho\\WebExplorer\\WebExplorerAPI\\src\\main\\java\\com\\example\\webexplorerapi\\Resolver";*/

    char classpath[] = "-Djava.class.path=D:/Zoho/WebExplorer/WebExplorerAPI/src/main/java/com/example/webexplorerapi/Resolver";

    JavaVM* jvm;
    JNIEnv* env;

    JavaVMInitArgs vm_args;
    JavaVMOption* options = new JavaVMOption[1];
    options[0].optionString = classpath;
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    delete options;
    if (rc != JNI_OK) {
        cin.get();
        exit(EXIT_FAILURE);
    }


    cout << "JVM load succeeded: Version" << endl;
    jint ver = env->GetVersion();

    jclass cls = env->FindClass("Resolver");

    cout << "Found Class " << (cls != nullptr);




}