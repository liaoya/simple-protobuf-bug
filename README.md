# README

This repo is used to reproduce a bug of <https://github.com/tonda-kriz/simple-protobuf>.

## How to build

```sh
rm -fr build
cmake -S . -B build
cmake --build build
```

## How to reproduce

Run `./build/empty-message-exe` show the json data is different (an empty message lost) after serialize and deserialize.
The problem is `simple-protobuf` does not serialize message

```text
json format after construct
{"version":"1.3","to_id":"oktopusController","from_id":"agent-1","websocket_connect":{}}
json format after deserialize
{"version":"1.3","to_id":"oktopusController","from_id":"agent-1"}
```

- `demo1()` reproduce my origin problem
- `demo2()` simple empty message is OK
- `demo3()` show (oneof) empty field lost
- `demo4()` show embed empty field lost
- `demo5()` show `simple-protobuf` can deserialize the message correct, so the problem is serializing message
