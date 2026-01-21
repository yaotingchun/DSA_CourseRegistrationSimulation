# 🎓 DSA Course Registration Simulation

> A robust, console-based university course registration system built with pure C++ to demonstrate the power of fundamental Data Structures and Algorithms.

## 🚀 Overview
This project simulates a real-world academic registry system. It handles students, courses, staff, and the intricate dance of course registrations. But unlike a typical CRUD app, this engine is built from scratch using raw data structures—no STL containers for the core logic (mostly)!

## 🧠 Under the Hood: Data Structures
We didn't just use arrays; we architected the system for efficiency and logic:

- **Binary Search Trees (BST)** 🌳
  - *Used for:* Storing **Students** and **Courses**.
  - *Why:* Enables fast `O(log n)` searching, insertion, and deletion. Whether looking up "A24CS0101" or "System Development", the search is lightning fast.

- **Queue (FIFO)** 🚶‍♂️🚶‍♀️
  - *Used for:* **Registration Requests**.
  - *Why:* First-Come, First-Served. When a student registers, their request enters a queue. Staff process these requests in the exact order they arrived, ensuring fairness.

- **Stack (LIFO)** 📚
  - *Used for:* **Notifications**.
  - *Why:* The most recent alerts (e.g., "Course Approved") appear at the top. Just like your email inbox or social media feed.

- **Linked Lists** 🔗
  - *Used for:* **Student's Registered Courses** and **Course's Student List**.
  - *Why:* Dynamic memory allocation allows lists to grow and shrink without pre-allocating memory. Perfect for variable-sized schedules.

## ✨ Features

### 👨‍🎓 For Students
- **Course Browsing**: View all available courses with credit and capacity details.
- **Smart Search**: Find courses instantly by code.
- **Registration**: Request to enroll in courses (adds to the Queue).
- **Schedule Management**: Drop courses or view your current confirmed schedule.
- **Notifications**: Check the status of your requests (Approved/Rejected).
- **Slip Printing**: Generate a text-based confirmation slip of your semester.

### 👩‍🏫 For Staff
- **Request Processing**: Review pending registration requests from the Queue and Approve/Reject them.
- **Course Management**: Add new courses, remove outdated ones, or modify capacities.
- **Student Oversight**: Search for any student, view their details, or modify their course list directly.
- **Analytics**: View class rosters and enrollment stats.

## 🛠️ Installation & Usage

1. **Compile**:
   Ensure you have a C++ compiler (like g++).
   ```bash
   g++ main.cpp -o course_system
   ```

2. **Run**:
   ```bash
   ./course_system
   ```

3. **Login**:
   - **Student Mode**: Enter a valid Matric No (e.g., `A24CS0101`).
   - **Staff Mode**: Enter a valid Staff ID (from `staff.txt`).

## 📂 File Structure
- `main.cpp`: The brain of the operation.
- `students.txt`: Database of student records.
- `courses.txt`: Database of course offerings.
- `staff.txt`: Authorized staff credentials.
- `pending_requests.txt`: Persistent queue state.

---
*Created as a project to explore the implementation of BSTs, Queues, Stacks, and Linked Lists in a practical application.*
