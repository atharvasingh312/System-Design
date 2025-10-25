# Database Sharding & Horizontal Partitioning

> Sources:  
> [What is Database Sharding? | AWS](https://aws.amazon.com/what-is/database-sharding/)  
> [Sharding vs Partitioning: DataCamp](https://www.datacamp.com/blog/sharding-vs-partitioning)

---

## What is Database Sharding?

**Database sharding** is the process of splitting a large database into smaller, independent segments called *shards*, each residing on different machines (database nodes). Every shard holds a subset of unique rows from the original data, but all shards share the same schema.

- **Purpose:** Enables massive scaling, parallel query processing, and isolation of failures.
- **Common use:** Large applications (social media, e-commerce, SaaS) that outgrow single-server capacity.

### Example  
Imagine a customer table:
| CustomerID | Name   | State     |
|------------|--------|-----------|
| 1          | John   | California|
| 2          | Jane   | Washington|
| 3          | Paulo  | Arizona   |
| 4          | Wang   | Georgia   |

After sharding:
- Shard A (Server 1): John, Jane
- Shard B (Server 2): Paulo, Wang

---

## How Does Sharding Work?

- **Shard Key:** A selected column (e.g. UserID, region) is used to determine assignment of each row to a specific shard.
- **Shared-Nothing Architecture:** Each shard is independent; failure or overload on one doesn't affect others.
- **Data Routing:** Handled by the application, middleware, or built-in DBMS logic.

---

## Sharding Methods

| Method              | Description                              | Example Usage         |
|---------------------|------------------------------------------|-----------------------|
| Range-Based Sharding| Data split by value ranges (e.g. IDs)    | A–I → Shard 1, J–S → Shard 2 |
| Hash-Based Sharding | Hash function evenly distributes rows     | hash(UserID)%N        |
| Directory-Based     | Lookup table assigns shards               | Product Category      |
| Geo Sharding        | Shards based on geography                 | State/Country         |

---

## Benefits of Sharding 

- **Improved Response Time:** Each shard has fewer rows, so queries are faster.
- **Fault Tolerance:** Failure in one shard does not mean total database outage.
- **Efficient Scalability:** Add/remove shards as app grows—no need for downtime.
- **Parallel Processing:** Shards can process queries independently for higher throughput.

---

## What is Horizontal Partitioning?

**Horizontal partitioning** is splitting a single table into smaller tables that each contain a subset of the rows, but reside on the same server or logical database.

- Used for query performance and data maintenance—not for actual horizontal scaling to new servers.
- All partitions have the same schema.

### Example  
Table `Users` split based on UserID:
- Partition 1: UserID 1–1000 (on Server A)
- Partition 2: UserID 1001–2000 (on Server A)

---

## Table: Horizontal Partitioning vs Sharding

| Feature                      | Horizontal Partitioning                | Sharding                                 |
|------------------------------|----------------------------------------|------------------------------------------|
| Location                     | Same server/database                   | Multiple distinct servers/databases      |
| Use Case                     | Query speed, maintenance               | Scalability, handling massive data       |
| Partition Key                | Row value(s)                           | Shard key (often same column)            |
| Fault Isolation              | Low (single server failure = full DB)  | High (each shard independent)            |
| Management                   | Easier, managed by DBMS                | More complex, needs software coordination|
| Schema                       | Same across partitions                 | Same across all shards                   |
| Scaling                      | Limited by single server               | No limit; add servers as needed          |
| Example                      | Table split into row ranges            | Table split & distributed across nodes   |
| Analytical Queries           | Can span all partitions                | Need to aggregate across shards          |

---

## Why Not to Confuse Sharding with Horizontal Partitioning?

- **Sharding = horizontal partitioning + distribution.**  
  While both split data by rows, *sharding* goes further by *distributing* partitions across different servers or databases.

- **Partitioning stays inside one logical DB**, and is meant for query or maintenance optimization—*sharding is a true scaling/fault isolation technique*.

---
