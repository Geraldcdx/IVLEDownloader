# IVLE Downloader - Developer Guide
## 1. Setting Up:
* 1.1 Prerequisites
* 1.2 Setting up the Project on your computer
## 2. Design:
 * 2.1 Architecture
 * 2.2 UI component
 * 2.3 Logic component
 * 2.4 Model component
## 3. Implementation:
 * 3.1 Login Implementation
 * 3.2 User Interface(UI)
 * 3.3 Files
 * 3.4 Announcements
 * 3.5 Exam Details
 * 3.6 Time Table
 * 3.7 CAP Calculator
 * 3.8 To-Do-List
## 4. Testing:
 * 4.1 Types of testing
 * 4.2 Conclusion
## 5. Dev Ops
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
 <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Archi.png" width="500"></p>
 

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
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Overview.png"width="700"></p>
   
 ## 2.2 UI component
 ### *The UI components work hand in hand with the logic component to get relevant information to display on the UI to allow users to view all offline IVLE information.*
 <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/UI.png" width="700"></p>
 
 ### The two main UI components:
  * **MainWindow.ui** : Sets up information such as API key, log in and directory to allow users to enjoy the full experience of the IVLE Downloader. This also provides a settings function that can be seen as a running application at the icons running.
  * **DownloaderUI.ui** : Contains the UI information for all the mentioned tabs above. For a detailed view, visit [User Guide](https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/Userguide.md).
 ## 2.3 Logic component
 Since this is written in C++ in OOP, there is always a .cpp file and .h file. If we want to create a UI, there will be a .ui file as well. Therefore, the way every .cpp,.h and .ui file is written to create a single class. In each class, different objects from different .h files can be called.
  <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Main.png" width="700"></p>
The following is the objects present in our MainWindow class:
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/MainO.png" width="700"></p>
   
 ### DownloaderUI.ui
This is the UI whereby files, announcements, exam details, timetable, CAPCalculator, to-do-list and NUSWhispers can be seen.
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/DownloaderUI.png" width="700"></p>
The following is the objects present in our DownloaderUI class:
   <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/DownloaderUIO.png" width="700"></p>
   
 ## 2.4 Model component
 Below is the exchanges between model and logic components to clarify the type of storage used in this application:
 <p align="center"><img src="https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/pics/Model.png" width="700"></p>
 
 This is a self-developed project, therefore, we experiemented with different kinds of data storage methods to learn more about the Qt library and external programs. The table below shows the different ways that can store data in Qt: 
 
 | No. | Method| Explanation |
| --- |:---| :-----|
| 1 | Local Directory |Qt has a library called QDir and this library enables Qt applications to read/write from/to local files|
| 2 | QSettings     | Qt has a library called QSettings. The QSettings class provides persistent platform-independent application setting|
| 3 | SQLite DB     |  Qt has a library called QtSQL. This class allows users to create SQLite DB and store information there|
 
## 3. Implementation:
### 3.1 Login Implementation
 * This code can be found in MainWindow.cpp.
 * This login implementation is mainly about ensuring every user has a unique API key to poll using the IVLE API. A common issue for users who use an API is revoking of API keys/tokens. When a user polls too much from a server, higher than the poll limit that is set by administrators, it will revoke the API key/token and render the API key/token useless.
 * A method of setting up first time users is necessary to ensure such a problem would not exist.
 * From the [User Guide](https://github.com/Geraldcdx/IVLEDownloader/blob/master/docs/Userguide.md) you will see the procedure of setting up.
 * A QWebView is created to load the URL to log in to generate a API key unique to every user of IVLE. A custom parsing code using QWebView, QWebFrame and QWebElements will extract the API key and save it into QSettings. 
 * When users log in, they poll from the API with their own API key.
### 3.2 User Interface(UI)
 * This code can be found in DownloaderUI.cpp.
 * The UI is created using QTabWidgets in the class DownloaderUI. Every tab is hardcoded to make a Files Tab, Announcements Tab, Exam Details Tab, TimeTable Tab, CAP Calculator Tab, To-Do-List Tab and NUSWhispers Tab.
 * The NUSWhispers Tab uses QWebview to display a browser with a URL, this is found in DownloaderUI class.
 * Code to display Files Tab is made using QFileModelSystem and QTreeView.
### 3.3 Files
 * 80% of the code here was found in https://github.com/yyjhao/IVLEDownloader. Bugs fixed were addressing the API Key issue in 3.1 and increasing the frequency of polling from 5 minutes to 1 hour.
 * Yu Jian implemented his own version of Promises in Promises.cpp similar to javascript. He then uses the promises throughout his code.
 * ExternalPageParser.cpp is implemented to parse workbin files in IVLE.
 * Settings.cpp is implemented to store setting by QSettings.
 * SettingsDialog.cpp is implemented to have a UI to let the user see the settings changed. It also creates a AdvanceDialog object from advanceDialog.cpp. This object is to allows user to view updates of files.
 * ExternalPageParser.cpp is implemented to parse pages,
 * IVLEFetcher.cpp is implemented to poll from the IVLE api and obtain files using the ExternalPageParser object and promises to provide concurrent downloading of files.
 ### 3.4 Announcements
 * Found in Announcements.cpp and called in DownloaderUI.cpp.
 * Uses parsing with QWebview, QWebFrame, QWebElement, IVLE API and qtJSON to parse module ID, module name and announcements.
 * Uses QDir to store announcements into a txt file.
 * Uses QTabWidgets to create announcements based on the number of modules present.
 ### 3.5 Exam Details
 * Found in Exam.cpp and called in DownloaderUI.cpp.
 * Uses parsing with QWebview, QWebFrame, QWebElement, IVLE API and qtJSON to parse announcements using module ID stored in 3.4.
 * Uses QTableWidget to make a custom number of tables based on number of modules.
 ### 3.6 Time Table
 * Found in TimeTable.cpp and called in DownloaderUI.cpp.
 * Uses QPixMap to display Time Table.
 ### 3.7 CAP Calculator
 * Found in CAPCalculator.cpp called in DownloaderUI.cpp.
 * Uses QDialog to create a dialog to allow user to input information in written in Dialog.cpp.
 * Uses QTableWidget to display CAP by user.
 * Custom code written to help students determine average score they need for future semesters.
 ### 3.8 To-Do-List
 * Found in ToDoList.cpp called in DownloaderUI.cpp.
 * Uses QDialog to allow users to input information written in addItem.cpp.
 * Uses QWidget to custom create widgets written in Item.cpp.
 * Uses QtSQL to interact with SQLite to have a database to store to-do-list information.
 * When a user adds an item, the item will be stored on the database. When the user changes the slider value, it will update the database.
 
## 4. Testing
### 4.1 Types of testing:
* Since it was done in the summer, there was no modules available. However, we requested help from professors to get CP2106 and CS2040 available to be used for testing.
* Unit tests targeting the lowest level methods/classes and ensured that each class functioned properly.
* Integration tests that are checking the integration of all code units and each class would work properly.
* Hybrids of unit and integration tests. These test are checking multiple code units as well as how the are connected together. The GUI and code worked together to achieve the intended outcome that we set.
### 4.2 Conclusion:
The testing of the IVLE Downloader has been successful.

## 5. Dev Ops
### For making a release and ready for deployment
