# Consistent Hashing

**Author:**  
Notes curated from [Hello Interview SWE YouTube](https://www.youtube.com/watch?v=vccwdhfqIrI) and [ByteByteGo](https://blog.bytebytego.com/p/consistent-hashing-101-how-modern).

***

## 1. Introduction

**Consistent hashing** is a foundational load balancing technique used in distributed systems to efficiently distribute and retrieve data across dynamic sets of servers, minimizing data reshuffling when servers are added or removed. Major systems (DynamoDB, Cassandra, Riak, Memcached) rely on it for scalable data placement.

***

## 2. The Problem with Traditional Hashing

When using **modulo-based hashing**:
- Each data item's key is hashed and distributed using `hash(key) % N`, where **N** is the number of servers.
- Adding or removing servers changes **N**, causing reallocation of nearly all data.
- When a fourth server is added, nearly every item's mapped server changes.
- Causes heavy data migration and possible downtime.

***

## 3. Consistent Hashing - The Solution

Consistent hashing organizes both servers and data keys around a **hash ring** (circular space from 0 to 2³²–1).

### Steps:
1. **Hash Ring:** Map servers (nodes) and keys onto a circular hash space.
2. **Data Placement:** When a key is inserted, hash it to a position; move clockwise to the next server. The key is stored on that server.
3. **Adding/Removing Servers:**
   - Only keys between the predecessor and the new server node need to move.
   - Minimal impact for cluster scaling, compared to the mod-hash approach.

## 4. Virtual Nodes for Load Balancing

**Why:** Data can be unevenly distributed if just a few servers exist (hash function artifacts).

**Solution:**  
- Each physical server is assigned multiple positions (**virtual nodes**) on the hash ring.
- Load is spread more evenly; if a physical server fails, only its virtual shares are redistributed.


- Prevents hot spots (overloaded servers).
- Ensures smooth scaling and failover.

***

## 5. Key Use Cases

- **Distributed Caches:** Memcached, Redis Cluster
- **NoSQL Databases:** DynamoDB, Cassandra
- **CDNs:** Large-scale content delivery and storage

***

## 6. C++ Implementation

Below is a basic C++ implementation of consistent hashing, including virtual nodes:

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

class ConsistentHashing {
    size_t virtualNodes;
    std::map<size_t, std::string> ring;
    std::hash<std::string> hashFunc;

public:
    ConsistentHashing(size_t vNodes = 100) : virtualNodes(vNodes) {}

    void addServer(const std::string& server) {
        for (size_t i = 0; i < virtualNodes; ++i) {
            std::string vnode = server + "#" + std::to_string(i);
            size_t hash = hashFunc(vnode);
            ring[hash] = server;
        }
    }

    void removeServer(const std::string& server) {
        for (size_t i = 0; i < virtualNodes; ++i) {
            std::string vnode = server + "#" + std::to_string(i);
            size_t hash = hashFunc(vnode);
            ring.erase(hash);
        }
    }

    std::string getServer(const std::string& key) {
        if (ring.empty()) return "";
        size_t hash = hashFunc(key);
        auto it = ring.lower_bound(hash);
        if (it == ring.end()) it = ring.begin();
        return it->second;
    }
};

int main() {
    ConsistentHashing ch(10); // 10 virtual nodes per server

    ch.addServer("ServerA");
    ch.addServer("ServerB");
    ch.addServer("ServerC");

    std::vector<std::string> keys = {"key1", "key2", "key3"};
    for (const auto& key : keys) {
        std::cout << "Key: " << key << " maps to " << ch.getServer(key) << std::endl;
    }

    ch.removeServer("ServerB");

    for (const auto& key : keys) {
        std::cout << "After removing ServerB, Key: " << key << " maps to " << ch.getServer(key) << std::endl;
    }

    return 0;
}
```
***

## 8. Further Reading & References

- [Consistent Hashing 101 by ByteByteGo](https://blog.bytebytego.com/p/consistent-hashing-101-how-modern)
- [Hello Interview SWE Youtube Explanation](https://www.youtube.com/watch?v=vccwdhfqIrI)