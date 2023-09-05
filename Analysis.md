# Lip Timer Project: Detailed Notes for Revision 📚🕒

## Introduction 🌟
This course project aims to use the time library developed earlier to solve a timer-based problem. The focus is on a Routing Table Manager application, which allows users to add, update, or delete routing table entries.

---

## Files Involved 📂
1. `ArtieDorsey` - Unknown function
2. `ArtieDaughters` - Implements the routing table
3. `ATTExpedition` - Main application file
4. Shell script (`compile.sh`) - To compile and create an executable

---

## Problem Statement 🎯
The Routing Table Manager application contains a static routing table with entries having an expiration time fixed at 30 seconds. The task is to implement timers for each routing table entry, such that the entry is deleted once its timer expires after 30 seconds.

---

## Routing Table Structure 📊
- Modeled as a linked list of routing table entries
- Each entry consists of:
  1. **Key**: Combination of `Destination Address` and `Mask Value` (unique)
  2. **Attributes**: `IP Address`, `Interface Name`, `Expiration Time`

---

## Steps to Solve the Problem ⚙️
1. **Step 1**: Add two new members in the routing table entry structure:
    - Expiration time (default 30 seconds)
    - Timer data structure
2. **Step 2**: Modify the existing API to add a new routing table entry by:
    - Including a new argument: A callback function to delete the entry when the timer expires
3. **Step 3**: Implement a timer callback function:
    - Delete the routing table entry and its associated timer when the timer expires

---

## Git Version Control 🛠️
1. For the solution, switch to branch `Project solution`.
2. Run `git log` and find the commit hash corresponding to `Solution to problem statement one` dated `27 October 1915 13`.
3. Run `git reset --hard [commit-hash]` to update the repo to that point in time.

---

# Interview Questions and Answers 🤔💡

### Q1: Can you explain the problem statement of the Lip Timer Project?

**A1**: The project focuses on implementing timers for each entry in a Routing Table Manager application. The aim is to have each entry automatically deleted after its timer expires, which is set at 30 seconds by default.

### Q2: How is the routing table implemented?

**A2**: The routing table is implemented as a linked list of routing table entries. Each entry is a node in the list, having unique keys composed of a `Destination Address` and `Mask Value`. Other attributes include the `IP Address`, `Interface Name`, and `Expiration Time`.

### Q3: What modifications are needed in the existing APIs to solve this problem?

**A3**: A new argument needs to be added to the existing API for adding a new routing table entry. This new argument will be a callback function that gets invoked when the timer associated with the entry expires, leading to the deletion of the entry.

### Q4: What role does the timer callback function play?

**A4**: The timer callback function is triggered when the timer associated with a routing table entry expires. It handles the logic for deleting the routing table entry as well as stopping its associated timer.

### Q5: How do you roll back to the solution using Git?

**A5**: You can roll back to the solution commit by first switching to the `Project solution` branch. Then, search for the specific commit hash using `git log` and reset to that commit using `git reset --hard [commit-hash]`.

---
# Many Project Analysis: Detailed Notes for Revision 📚🕒

## Introduction 🌟
This video delves into the analysis of the "Many Project," which revolves around the Routing Table Manager application. The analysis lays the foundation for the next course section.

---

## Independent Timers for Each Routing Table Entry 🕒
- Each routing table entry has its independent timer.
- Timer acts as an independent thread.
- If there are 5 entries, there are 5 threads running timers for each entry.

---

## Concurrency Problem 🔀
- Concurrency issues arise during the deletion of routing table entries.
- When multiple timers expire at the same time, multiple threads try to delete their respective entries from the same routing table.
- May result in segmentation faults or unexpected behaviors due to "write-side conflicts."

### Solution: Mutex 🔒
- Utilize mutex to prevent concurrency problems.

---

## Scalability Concerns 📈
- The current design is not scalable.
- For instance, it can't manage one million routing table entries with individual timers.
- One machine can't handle one million threads.

---

## Introducing Wheel Timers 🎡
- A new timer architecture called "Wheel Timers."
- Designed to scale effectively.
- Can handle millions of entries with just one thread.

### Features 🌈
- Runs in the context of a single thread.
- Eliminates the concurrency problem.
- Sequential deletion of entries when multiple entries expire at the same time.

### How it Works 💡
- Integrates existing "Linux wrapper Timer" with a new data structure called "Wheelers structure."
- Resultant product: "Wheel Timer"

---

# Interview Questions and Answers 🤔💡

### Q1: Can you explain the concurrency problem in the Many Project?

**A1**: The Many Project has a concurrency issue where multiple threads try to delete their respective routing table entries from the same routing table when the timers expire simultaneously. This may lead to write-side conflicts and unexpected behaviors.

### Q2: How does the Many Project intend to solve the scalability issue?

**A2**: The project plans to introduce a new timer architecture called "Wheel Timers." This design can manage millions of routing table entries with just one thread, making it highly scalable.

### Q3: What are Wheel Timers and how do they resolve the concurrency issue?

**A3**: Wheel Timers are a new type of timers designed to be scalable. They operate in the context of a single thread, thereby eliminating the concurrency issue. When multiple entries expire, they are deleted sequentially, avoiding conflicts.

### Q4: How do Wheel Timers integrate with the existing Linux wrapper Timer?

**A4**: Wheel Timers are not entirely new timers. They integrate the existing Linux wrapper Timer with a new data structure called "Wheelers structure," effectively scaling the timer capabilities.

### Q5: What is the role of a mutex in solving the concurrency problem in the Many Project?

**A5**: A mutex can be used to lock the critical section of the code that deletes routing table entries. This ensures that only one thread can delete an entry at any given time, preventing write-side conflicts and concurrency issues.

---

Best of luck with your interviews! 🍀👍
Best of luck with your interviews! 🍀👍
