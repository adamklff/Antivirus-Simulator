# Antivirus Simulator

A C command-line exercise in signature matching, linked lists, and memory management. Enter sample text, define signatures, then scan and move matching samples between in-memory lists.

## Build and try it

Requires GCC and Make. Run `make`, then `./toy_antivirus`.

```text
ADD_SIG DEMO_MARKER
LOAD notes Ordinary sample
LOAD test This sample contains DEMO_MARKER
SCAN
REPORT
QUAR test
REPORT
RESTORE test
EXIT
```

`SCAN` searches the Clean list for case-sensitive byte sequences. `QUAR` moves a suspect sample into the Quarantine list. `RESTORE` returns a quarantined sample to Clean, where it can be scanned again. `PURGE` clears all samples and signatures. Quarantine is an in-memory state and the CLI accepts typed sample text.

## Credit

This is a group coursework fork of [creepout777/antivirus-simulator](https://github.com/creepout777/antivirus-simulator). The original task allocation assigned data structures and prototypes to Yassine, file-list management to Adam, signatures to Marwa, scanning to Malak, and cleanup to Hamza. The runnable follow-up was completed with AI assistance.

Licensed under the [MIT License](LICENSE).
