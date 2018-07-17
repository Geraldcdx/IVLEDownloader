# IVLE Downloader - Developer Guide
## 1. Setting Up:
* 1.1 Prerequisites
* 1.2 Setting up the Project on your computer
## 2. Design:
 * 2.1 Architecture
 * 2.2 UI component
 * 2.3 Logic component
 * 2.4 Model component
 * 2.5 Storage component
 * 2.6 Common classes
 

## 1. Setting Up:
#### 1.1 Prerequisites:
  * First of all you have to download Qt Creator from https://www.qt.io/download. 
  * **Important**: to download any version of Qt 5.4 or lower. This is because the orignal code written here https://github.com/yyjhao/IVLEDownloader uses QWebKit which is library used by Qt 5.4 or lower.
##### 1.2 Setting up the Project on your computer
 * Open the whole project file using Qt Creator
 * Follow the picture below and press the green + button to add your version of Qt installed. Then, you will be able to build and run.
<img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/projectc.png" width="250">

## 2. Design:
 #### 2.1 Architecture
 ![Alt-Text](https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Archi.png)
 ##### The **Architecture Diagram** given above explains the high-level design of the App. Given below is a quick overview of each component.
 ##### When the app is executed, the first time users will be called to extract their APIKey and login. Users that has logged in and done set-up procedures will be brought into a UI.
 ##### The 3 components of the application that follow the Model View Controller(MVC) model are:
  * **Logic** - This is the code that allows everything to function. It acts as a controller.
  * **UI** - This is how information is displayed to the user. It acts as the view.
  * **Model** - This is the database formats used in the application.
 ##### This is the behind the seens of the application:
  
 #### 2.2 UI component
 #### 2.3 Logic component
 #### 2.4 Model component
 #### 2.5 Storage component
 #### 2.6 Common classes

