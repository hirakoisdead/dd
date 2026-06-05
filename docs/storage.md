# Storage Format

This document describes how repository data is represented and stored inside `dd`.

## Object Database

All repository data is stored as immutable objects inside:

```text
.vcs/objects/
```

Each object is identified by a SHA-1 hash derived from its contents.

Objects are stored using the following layout:

```text
.vcs/objects/ab/cdef1234...
```

Where:

* `ab` = first two characters of the hash
* `cdef1234...` = remaining characters

## Generic Object Format

Before hashing, every object is serialized as:

```text
<type>\0<data>
```

Examples:

```text
blob\0Hello World
```

```text
tree\0...
```

```text
commit\0...
```

The resulting byte sequence is hashed and stored in the object database.

## Blob Objects

A blob stores file contents.

Structure:

```text
blob
└── raw file bytes
```

Example:

File:

```text
README.md
```

Contents:

```text
Hello World
```

Serialized object:

```text
blob\0Hello World
```

The resulting hash uniquely identifies the file contents.

Identical file contents produce identical blob hashes.

## Tree Objects

A tree stores directory structure.

Structure:

```text
tree
├── README.md -> blob
└── src -> tree
```

Each tree entry contains:

```text
<type> <name>\0<hash>
```

Where:

* `type` = `blob` or `tree`
* `name` = file or directory name
* `hash` = referenced object hash

Example:

```text
blob README.md\0a94f3c...
tree src\0f29ab1...
```

Trees allow an entire directory hierarchy to be represented by a single hash.

## Commit Objects

A commit stores repository history.

Structure:

```text
commit
├── tree
├── parent
├── author
├── timestamp
└── message
```

Serialized format:

```text
tree <tree_hash>
parent <parent_hash>
author <author>
timestamp <unix_timestamp>

<message>
```

Example:

```text
tree a94f3c...
parent b712aa...
author Muaviz
timestamp 1760000000

Initial commit
```

The first commit omits the parent field.

Each commit references a complete repository snapshot through its root tree.

## Index Format

The index acts as the staging area.

Location:

```text
.vcs/index
```

Each entry maps:

```text
path -> blob hash
```

Stored as:

```text
<hash> <path>
```

Example:

```text
a94f3c91ab README.md
f29ab193df src/main.cpp
```

The index represents the exact contents that will be included in the next commit.

Unlike repository objects, the index is mutable and may be rewritten frequently.

## Repository Relationships

```text
Working Directory
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

A commit references a root tree.

A tree references blobs and subtrees.

Blobs store file contents.

Branches store commit hashes.

Together these objects form the complete repository history.
