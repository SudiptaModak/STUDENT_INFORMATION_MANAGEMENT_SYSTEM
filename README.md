# Student Information Management System (SIMS)

A robust, terminal-based **Student Information Management System** engineered in C. This application streamlines student academic tracking, handles multi-semester performance analytics (SGPA/CGPA), and maintains fully persistent storage using low-level binary file handling. 

To optimize searching and sorting pipelines, the system reads records dynamically into heap memory and couples standard library sorting (`qsort`) with an optimized **Binary Search** engine for high-speed record lookups, updates, and deletions.

---

## 🚀 Key Features

* **💾 Persistent Binary Storage:** Records are securely written to and loaded from a flat binary data file (`students.dat`), ensuring zero data loss between system restarts.
* **🆔 Smart ID Management:** Implements an automated, sequential system ID allocation tracker synced with an independent metadata file (`hifhestID.dat`) to preserve state across sessions.
* **🛡️ Integrity Constraints:** Built-in validation structures that prevent duplicate entries for the same Roll Number and enforce boundaries for academic metrics.
* **📈 Academic Analytics Engine:** Dynamically tracks performance across up to 8 semesters, computing live `Current CGPA` milestones and finalizing a global `Graduation CGPA` upon degree completion.

---

## 🧠 Core Architecture & Data Structures

The system architecture relies on structured contiguous records designed for clean block writes (`fwrite`) and sequential block reads (`fread`):

```c
struct Student {
    int roll;               // Unique Roll Number
    char name[50];          // Student Name
    int age;                // Age
    int completedSem;       // Current completed semesters count (1-8)
    long id;                // Unique, system-generated tracking ID
    float sgpa[8];          // Array storing individual semester performance
    float currentCGPA;      // Dynamic average of completed semesters
    float cgpa;             // Final Degree CGPA (-1 if undergraduate incomplete)
};


System Modules & Functionality
The application features an interactive console dashboard providing six primary operations:Add Student Record: Validates semester boundaries, screens for preexisting roll numbers to prevent collisions, issues an auto-incremented ID, and appends the object directly to disk.Display All Records: Parses the underlying binary structures into human-readable transcripts, displaying semester timelines alongside progress statuses.Search Student by ID: Leverages dynamic memory allocation to extract files into an array, applies a quick-sort pass, and uses a $O(\log n)$ Binary Search algorithm to isolate specific profiles.Update Student Record: Isolates target profiles instantly via binary search, permitting in-place modifications to core demographics or the trailing addition of new semester grades.Delete Student Record: Filters out targeted record blocks during memory serialization and rewrites the remaining clean dataset back to students.dat.Exit & Safe-State Write: Safely flushes state variables and the highest generated tracking ID to disk before cleanly closing the program environment.
