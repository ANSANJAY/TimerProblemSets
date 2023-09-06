# TimerProblemSets
This repository is a collection of problem statements focused on timer-based scenarios, each with corresponding solutions and explanations. Ideal for learners, enthusiasts, and professionals, this resource aims to offer a deep understanding of time-related algorithms and best practices. Examples range from simple to advanced.


#  Timer-based Routing Table Management 🕒💻

## Overview 📋
This  discusses a timer-based problem focused on routing table management. It showcases how to integrate a time library into an existing project to implement an automatic deletion feature for routing table entries after a set time interval of 30 seconds.

---

## Concepts Explained 📚

### Timer Basics 🕐
- **Timer Resumption**: A timer that has been paused can be resumed by repopulating the DSC attribute with the remaining time.
- **Timer State**: After resumption, the timer moves from a "paused" state to a "resumed" state and then to a "running" state after its first expiration.


### Problem Statement 📝
- Implement a routing table manager that automatically deletes entries after 30 seconds.
- Routing Table Entries: Each entry has a `destination address`, `mask value`, `IP address`, `outgoing interface name`, and `time to expire`.

### Data Structure 🏗
- The routing table is modeled as a linked list of entries.
- Each entry has a key (`destination address` and `mask value`) and attributes (`IP address`, `outgoing interface name`, `time to expire`).

---

## Interview Questions and Answers ❓💡

### Q1: How do you resume a paused timer?
  **Answer**: To resume a paused timer, you populate the DSC attribute with the time remaining. The timer then transitions from a "paused" state to a "resumed" state.

### Q2: How is the routing table implemented in the project?
  **Answer**: The routing table is implemented as a linked list of entries. Each entry contains a key (destination and mask value) and several attributes (IP address, outgoing interface name, time to expire).

### Q3: What happens when the timer expires for a routing table entry?
  **Answer**: When the timer expires, the corresponding routing table entry is automatically deleted.

### Q4: How are new entries added to the routing table?
  **Answer**: New entries are added using the API `routing_table_add_new_routing_table_entry`, which takes all the necessary parameters for a complete routing table entry.

### Q5: What are the essential fields required to delete an entry from the routing table?
  **Answer**: To delete an entry, you need to specify the `destination IP address` and `mask value` as these two together form the key for each routing table entry.

---

Feel free to revisit these notes for your interviews! Good luck! 🍀
