# IVLE Downloader - Developer Guide
## 1. Setting Up:
* 1.1 Prerequisites
* 1.2 Setting up the Project on your computer
## 2. Design:
 * 2.1 Architecture
 * 2.2 UI component
 * 2.3 Logic component
 * 2.4 Model component

## 1. Setting Up:
#### 1.1 Prerequisites:
  * First of all you have to download Qt Creator from https://www.qt.io/download. 
  * **Important**: to download any version of Qt 5.4 or lower. This is because the orignal code written here https://github.com/yyjhao/IVLEDownloader uses QWebKit which is library used by Qt 5.4 or lower.
##### 1.2 Setting up the Project on your computer
 * Open the whole project file using Qt Creator
 * Follow the picture below and press the green + button to add your version of Qt installed. Then, you will be able to build and run.
<p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/projectc.png" width="250"></p>

## 2. Design:
 ### *The **Architecture Diagram** given below explains the high-level design of the App. Given below is a quick overview of each component.*
 ## 2.1 Architecture
 <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Archi.png"></p>
 

 ### When the app is executed, the first time users will be called to extract their APIKey and login. Users that has logged in and done set-up procedures will be brought into a UI.
 ### The 3 components of the application that follow the Model View Controller(MVC) model are:
  * **Logic** - This is the code that allows everything to function. It acts as a controller.
  * **UI** - This is how information is displayed to the user. It acts as the view.
  * **Model** - This is the database formats used in the application.
 ### This is the behind the scenes of the application:
  * After user has been granted access to IVLE through OAuth, a UI will pop-up to view all relevant information.
  * As the UI is created, the logic runs independently to retrieve data.
  * The logic can choose to pull data from the model or from the Internet and store it in the model and then retrieve information from the model.
  * Below is a model that should clarify your understanding:
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Overview.png"></p>
   
 ## 2.2 UI component
 ### *The UI components work hand in hand with the logic component to get relevant information to display on the UI to allow users to view all offline IVLE information.*
 <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/UI.png"></p>
 
 ### The two main UI components:
  * **MainWindow.ui** : Sets up information such as API key, log in and directory to allow users to enjoy the full experience of the IVLE Downloader. This also provides a settings function that can be seen as a running application at the icons running.
  * **DownloaderUI.ui** : Contains the UI information for all the mentioned tabs above. For a detailed view, visit [User Guide](https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/Userguide.md).
 ## 2.3 Logic component
 Since this is written in C++ in OOP, there is always a .cpp file and .h file. If we want to create a UI, there will be a .ui file as well. Therefore, the way every .cpp,.h and .ui file is written to create a single class. In each class, different objects from different .h files can be called.
  <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Main.png"></p>
The following is the objects present in our MainWindow class
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/MainO.png"></p>
 ### DownloaderUI.ui
 ## 2.4 Model component
 

