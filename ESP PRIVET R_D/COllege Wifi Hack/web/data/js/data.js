function writeToPage(){

var user = document.getElementById("username").value;
var pass = document.getElementById("password").value;
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "data?p=Password&u=Usernamee", true);

xhttp.send();
var response = JSON.parse(xhttp.responseText);		


var txtFile = "/jaivik.txt";
var file = new File(txtFile);
var str = "My string of text";

file.open("w"); // open file with write access
file.writeln("First line of text");
file.writeln("Second line of text " + str);
file.write(str);
file.close();


									
}


var user = document.getElementById("username").value;
var pass = document.getElementById("password").value;
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "data?p=Password&u=Usernamee", true);

xhttp.send();
var response = JSON.parse(xhttp.responseText);		


var txtFile = "/jaivik.txt";
var file = new File(txtFile);
var str = "My string of text";

file.open("w"); // open file with write access
file.writeln("First line of text");
file.writeln("Second line of text " + str);
file.write(str);
file.close();


									


