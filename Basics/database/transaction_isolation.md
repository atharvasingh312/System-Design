# Transaction Isolation, Anomalies, and Concurrency Control

## 1. Isolation Levels in DBMS
Isolation levels determine how/when changes from one transaction are visible to others and what concurrency anomalies can occur:

| Isolation Level      | Dirty Read | Non-Repeatable Read | Phantom Read |
|---------------------|:----------:|:-------------------:|:------------:|
| Read Uncommitted    |    Yes     |        Yes          |     Yes      |
| Read Committed      |    No      |        Yes          |     Yes      |
| Repeatable Read     |    No      |        No           |     Yes      |
| Serializable        |    No      |        No           |     No       |

### Isolation Level Details:
- **Read Uncommitted:** No locks on reads. Transactions see even uncommitted changes. All anomalies possible.
- **Read Committed:** Reads only committed data; dirty reads prevented. Other anomalies possible as read locks are released after each read.
- **Repeatable Read:** Holds read locks for rows until transaction ends. Dirty/non-repeatable reads blocked, but phantoms allowed since range locks not used for predicates.
- **Serializable:** Strictest. Transactions executed as if serially (one after another). Uses strict two-phase locking or MVCC with serializability checks. No anomalies allowed.

---

## 2. Concurrency Anomalies Explained with Examples
- **Dirty Read:** A transaction reads uncommitted data from another transaction. Example: Transaction A updates a value but does not commit. Transaction B reads this new value. If A rolls back, B has read a value that never truly existed.
- **Non-Repeatable Read:** A transaction reads the same row twice and gets different results due to another transaction’s update. Example: Transaction A reads row X. Transaction B updates row X and commits. A reads row X again and sees a different value.
- **Phantom Read:** Query returns different sets of rows due to insert/delete by other transactions. Example: Transaction A queries for orders > INR 1000, finds 3. Transaction B inserts matching order, A re-runs the query and finds 4.

---

## 3. How Isolation Mechanisms Allow or Prevent Anomalies
- **Locks:** Isolation levels control lock strength/duration. Lower levels use fewer/weaker locks = more anomalies.
- **MVCC:** Multi-version concurrency control creates multiple versions for each data item; transactions interact with versions according to timestamp.
- **Two-Phase Locking (2PL):** Serializable isolation uses full locking until transaction ends or advanced snapshot isolation + predicate/range locks.
- **Tradeoff:** Higher isolation = fewer anomalies but less concurrency/performance.

---

## 4. Serializable Isolation Level
- **Prevents all anomalies** through:
    - Strict two-phase locking (row/range/predicate locks held until commit)
    - Serial execution or conflict detection (MVCC with aborts/validation)
- If a transaction might create an anomaly (e.g. update seen by future transaction, inserts matching ongoing query), it is blocked or aborted.
- Used for highest consistency (banking, finance, etc.)

---

## 5. Timestamp-based MVCC Explained
- **Goal:** High concurrency, consistent snapshots for all transactions.
- **Each transaction** assigned a unique timestamp (logical start time).
- **Each data item** maintains multiple versions, each tagged with its write timestamp.
- **Reads:** Transaction reads the latest version whose write timestamp is ≤ its own timestamp.
- **Writes:** Transaction can only write if its timestamp ≥ all relevant read/write timestamps; otherwise, aborted.

### MVCC Read Algorithm
1. To read(X), find version with write TS ≤ transaction TS.
2. If none found, abort (future version).

### MVCC Write Algorithm
1. Abort if transaction TS < any read/write TS on versions of X (conflict with newer transaction).
2. Else, create new version with own timestamp.

---

## 6. MVCC: Easy Example
- T1 starts at 10, T2 at 20.
- T1 writes X at 10; T2 reads X at 20 (sees version X_10).
- If T1 tries to write to X after T2 reads it with newer TS, T1 is aborted -- serializability enforced!

---

## 7. Concurrency Control Summary Table
| Operation   | When Allowed                              | When Rejected (Abort)   |
|-------------|-------------------------------------------|-------------------------|
| Read(X)     | Read version with W_TS ≤ transaction TS   | No such version         |
| Write(X)    | TS ≥ all R_TS, W_TS on all versions       | TS < R_TS or TS < W_TS  |

MVCC delivers high concurrency and is the engine powering many modern distributed/analytical databases (PostgreSQL, CockroachDB, etc).
