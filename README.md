# dd

A Git-inspired version control system written in C++ for Linux.

`dd` was built from scratch to understand how modern version control systems work internally. Instead of relying on existing  libraries, the project implements core concepts such as content-addressable storage, immutable objects, commit history, branching, checkout, and repository reconstruction.

## Features

* Initialize and manage repositories.
* Store data using content-addressable object storage.
* Support blob, tree, and commit objects.
* Stage and track file changes through an index.
* Create commits and traverse repository history.
* Create, list, and switch between branches.
* Checkout commits or branches and reconstruct the working tree.

## Example Usage

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/614d3fe9-7ee3-44cf-82be-8e62d591b111" />

## Repository Layout

```text
.dd/
├── objects/
│
├── refs/
│   └── heads/
│
├── HEAD
├── index
└── config
```

### Components

| Component   | Purpose                              |
| ----------- | ------------------------------------ |
| objects/    | Stores all immutable objects         |
| refs/heads/ | Branch references                    |
| HEAD        | Current checked-out branch or commit |
| index       | Staging area                         |
| config      | Repository configuration             |

## Building

### Requirements

* Linux
* CMake
* C++17 compatible compiler

### Build

```bash
git clone <repository>
cd dd

mkdir build
cd build

cmake ..
cmake --build .
```

## Usage

### Initialize Repository

```bash
dd init
```

### Stage Files

```bash
dd add file.txt
```

### Create Commit

```bash
dd commit -m "initial commit"
```

### View History

```bash
dd log
```

### Create Branch

```bash
dd branch dev
```

### List Branches

```bash
dd branch
```

### Switch Branch

```bash
dd checkout dev
```

### Checkout Commit

```bash
dd checkout <commit_hash>
```

## Key Learnings

During development, the project explored:

* Content-addressable storage
* Hash-based object databases
* Snapshot-based version control
* Commit graph traversal
* Recursive tree structures
* Filesystem manipulation
* Layered software architecture
* CLI application design
* Persistent storage systems

## Future Work

Potential future extensions include:

* Diff engine
* Three-way merge
* Merge commits
* Tags
* Object compression
* Packfiles
* Remote repositories
* Push / Pull support
* Clone support
* Conflict resolution
* Detached HEAD improvements

## Acknowledgements

This project was inspired by the design principles of Git and was developed as an educational exploration of version control internals and systems programming in C++.
