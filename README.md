# Data-Structures-and-Algorithms-Project
## Project Discription
Our project consists of:
-	Reading an XML file and be able to detect and correct syntax errors.
-	Prettifying the XML file.
-	Converting the XML file to JSON.
-	Minifying the XML file.
-	Compressing the data in the XML file.

In our system the XML file will represent users in a social network Each user has id (unique), name, list of posts, list of followers. Each post has text and list of topics.
-	representing the user’s data using the graph data structure.
-	Network analysis.
-	Post Search.
-	Graph visualization.
All these functionalities should be accessed from a GUI
## Project Map
"XML Reader Codes" folder incudes:
-	XMLNode class header and c++ codes (it represents the nodes of the tree)
-	XMLDocument class header and c++ codes (contains the function for loading, minimizing, error correction and detection, writing, converting to JSON, and converting to graph).
-	Graph class header and c++ codes (contains the XML graph and network analysis functions) Note: most famous user function exists in XMLDocument class.

"Compression" folder includes:
-	Codes regarding compression and decompression.

"GUI" folder includes:
-	Codes regarding GUI.

##Algorithm
![image](https://user-images.githubusercontent.com/104307082/216668671-b0606115-2803-42f2-bce9-a3abe6b01d83.png)

#First:
You can write an XML code in the text area shown or you can load an XML file by pressing on choose file button.

