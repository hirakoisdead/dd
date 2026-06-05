# Architecture

The system is organized into several layers, each with a single responsibility.

## High-Level Workflow

```text
Working Directory
        │
        ▼
       add
        │
        ▼
      Index
        │
        ▼
      Trees
        │
        ▼
     Commits
        │
        ▼
      Branches
```

### Working Directory

The working directory contains the files currently visible to the user.

Example:

```text
project/
├── main.cpp
├── README.md
└── src/
```

These files are not stored directly in the repository database until they are staged.

### Index

The index acts as a staging area.

It stores mappings of:

```text
path -> blob hash
```

Example:

```text
a94f3c... README.md
f29ab1... src/main.cpp
```

The index represents the exact contents that will be included in the next commit.

### Trees

Trees represent directory structures.

A tree contains references to:

* Blob objects (files)
* Other tree objects (subdirectories)

Example:

```text
root
├── README.md
└── src
    └── main.cpp
```

Trees allow the entire repository structure to be represented as immutable objects.

### Commits

A commit represents a repository snapshot.

Each commit references:

* A root tree
* A parent commit (optional)
* Metadata
* Commit message

Example:

```text
Commit
│
├── Tree
│
├── Parent Commit
│
├── Author
│
├── Timestamp
│
└── Message
```

### Branches

Branches are lightweight references.

A branch stores only a commit hash.

Example:

```text
main -> C3
dev  -> C5
```

Branches do not store files, trees, or history.

They are simply movable pointers.

## Design Principles

### Content Addressability

Objects are identified by the hash of their contents.

Identical data always produces the same identifier.

### Immutability

Objects are never modified after creation.

Any change creates a new object.

### Snapshot-Based History

Commits represent complete repository snapshots rather than file-by-file changes.
