This C program connects to NASA’s DONKI (Database Of Notifications, Knowledge and Information) API to retrieve and display information about space weather events, especially Coronal Mass Ejections (CMEs). It uses libcurl to make HTTP requests and cJSON to parse the returned JSON data. The program is structured to be easily extendable to include other events such as Solar Flares, SEP (Solar Energetic Particles), and Geomagnetic Storms.

Features
Fetches space weather data from NASA’s DONKI API

Displays detailed information about Coronal Mass Ejections (CMEs), including:

Start time of the event

Source location on the Sun

Instruments that recorded the event

Video links, if available

Clean and organized terminal output

Written in standard C

Designed to be extendable for additional space weather event types

Requirements
C Compiler (like GCC)

libcurl (for HTTP requests)

cJSON (for parsing JSON)

To install dependencies on Debian/Ubuntu:

sudo apt update
sudo apt install libcurl4-openssl-dev libcjson-dev

Compilation
To compile the program:

gcc event_space_weather.c -o event_space_weather -lcurl -lcjson

Usage
To run the program:

./event_space_weather

It will connect to NASA’s API and retrieve CME events from July 1st to July 5th, 2025. You can change the date range by modifying the URL in the source code.

Example Output
☀️ CME Events (Coronal Mass Ejections):

📅 Start Time: 2025-07-03T12:48Z
🌍 Source Location: N18W22
🔭 Instruments: SOHO LASCO C2
🎞️ Video: https://iswa.gsfc.nasa.gov/downloads/...
Supported Event Types (for future updates)
The DONKI API supports several event types. This program currently supports only CME, but can be extended to include:

FLR – Solar Flares

SEP – Solar Energetic Particles

GST – Geomagnetic Storms

HSS – High Speed Streams

RBE – Radio Blackout Events

MPC – Magnetopause Crossing

Each type has its own endpoint in the DONKI API.

API Key
The program uses NASA’s default DEMO_KEY. For production or extended usage, you should register for your own API key at https://api.nasa.gov and replace the line in the code:

#define API_KEY "YOUR_OWN_KEY"

File Structure
event_space_weather.c → Main C source file

README.md → Documentation file (this one)

Credits
NASA API: https://api.nasa.gov

DONKI documentation: https://kauai.ccmc.gsfc.nasa.gov/DONKI/

Tools used: libcurl, cJSON

License
This project is open source and free to use under the MIT license. NASA data is publicly accessible for educational and research use.
