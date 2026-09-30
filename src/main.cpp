#include <format>
#include <fstream>
#include <iostream>
#include <vector>

#include "proto/demo.pb.h"
#include "proto/usp-record-1-4.pb.h"
#include "spb/pb.hpp"

std::vector<std::byte> read_binary_file(std::string_view filename) {
    std::ifstream file(filename.data(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("failed to open {}", filename));
    }
    file.seekg(0, std::ios::end);
    auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<std::byte> buffer(fileSize);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
        throw std::runtime_error(std::format("failed to read {}", filename));
    }
    return buffer;
}

usp_record::Record sp_connect() {
    usp_record::Record record;
    record.version = "1.3";
    record.to_id = "oktopusController";
    record.from_id = "agent-1";
    record.record_type = usp_record::WebSocketConnectRecord{};
    return record;
}

bool write_bytes_to_file(const std::string &filename, const std::vector<std::byte> &data) {
    std::ofstream outfile(filename, std::ios::out | std::ios::binary);

    if (!outfile) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    if (!data.empty()) {
        outfile.write(reinterpret_cast<const char *>(data.data()), data.size());
    }

    return outfile.good();
}

void demo1() {
    std::cout << "\ndemo1" << std::endl;
    auto record = sp_connect();
    std::cout << "json format after construct" << std::endl;
    std::cout << spb::json::serialize(record) << std::endl;

    std::vector<std::byte> vec;
    spb::pb::serialize(record, vec);

    usp_record::Record record1;
    spb::pb::deserialize(record1, vec);
    std::cout << "json format after deserialize" << std::endl;
    std::cout << spb::json::serialize(record1) << std::endl;

    auto filename = "record.pb";
    write_bytes_to_file(filename, vec);
    std::cout << "write to " << filename
              << " and run 'protoc -I proto --decode usp_record.Record usp-record-1-4.proto <'" << filename
              << " to check its value" << std::endl;
}

void demo2() {
    std::cout << "\ndemo2" << std::endl;
    auto msg = demo::Empty{};
    std::cout << "Empty json format after construct" << std::endl;
    std::cout << spb::json::serialize(msg) << std::endl;

    std::vector<std::byte> vec;
    spb::pb::serialize(msg, vec);

    demo::Empty msg1;
    spb::pb::deserialize(msg1, vec);
    std::cout << "Empty json format after deserialize" << std::endl;
    std::cout << spb::json::serialize(msg1) << std::endl;
}

void demo3() {
    std::cout << "\ndemo3" << std::endl;
    auto msg = demo::Combine();
    msg.id = "id";
    msg.body = demo::Empty{};
    std::cout << "Combine json format after construct" << std::endl;
    std::cout << spb::json::serialize(msg) << std::endl;

    std::vector<std::byte> vec;
    spb::pb::serialize(msg, vec);

    demo::Combine msg1;
    spb::pb::deserialize(msg1, vec);
    std::cout << "Combine json format after deserialize" << std::endl;
    std::cout << spb::json::serialize(msg1) << std::endl;
}

void demo4() {
    std::cout << "\ndemo4" << std::endl;
    auto msg = demo::Embed();
    msg.id = "id";
    msg.empty = demo::Empty{};
    std::cout << "Embed json format after construct" << std::endl;
    std::cout << spb::json::serialize(msg) << std::endl;

    std::vector<std::byte> vec;
    spb::pb::serialize(msg, vec);

    demo::Embed msg1;
    spb::pb::deserialize(msg1, vec);
    std::cout << "Embed json format after deserialize" << std::endl;
    std::cout << spb::json::serialize(msg1) << std::endl;
}

void demo5() {
    std::cout << "\ndemo5: Deserialize correct" << std::endl;
    auto filename = "pb/connect.pb";
    auto vec = read_binary_file(filename);

    usp_record::Record record1;
    spb::pb::deserialize(record1, vec);
    std::cout << "Deserialize the message correctly" << std::endl;
    std::cout << "Run 'protoc -I proto --decode usp_record.Record usp-record-1-4.proto <" << filename << "' to verify"
              << std::endl;
    std::cout << spb::json::serialize(record1) << std::endl;
}

int main() {
    demo1();
    demo2();
    demo3();
    demo4();
    demo5();
}
