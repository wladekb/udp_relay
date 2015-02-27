## UDP relay

Simple UDP relay that supports multiple destinations. All received data is sent to all destinations.

## Installation

1. Download source
2. Run **make**

## Running

```
udp_relay <source> <destination_1> [ <destination_N ...> ]
    where <source> and <destination> format is: <ip>:<port>
```
eg.
```
./udp_relay 0.0.0.0:1088 127.0.0.1:1089 127.0.0.1:1090
```
