# How to contribute

Thank you for contributing to *Embedded-Function*!

To make sure the process of accepting patches goes smoothly for everyone (especially for the maintainer), you should try to follow these few simple guidelines when you contribute:

0. Before contributing, please open an issue to discuss significant changes.

1. Fork the repository.

2. Create a new branch based on the latest `version-x.y.z` branch. (If `version-x.y.z` branch does not exist, basing on the `main` is OK)

```bash
  git checkout -b your_branch_name version-x.y.z
  # git checkout -b your_branch_name main
  # ATTENTION: your_branch_name cannot start with `version-`
```

3. Do your modifications on that branch. Except for special cases, your contribution should include proper unit tests and documentation.

4. Make sure your modifications did not break anything by building and running the [tests](./test/README.md).

5. Commit your changes. Your commit message should start with a one line short description of the modifications, with the details and explanations of your modifications following in subsequent paragraphs or bullet points.

6. Push the changes to your fork (`git push origin your_branch_name`).

7. Open a pull request against *Embedded-Function*'s latest `version-x.y.z` branch (or `main` branch if `version-x.y.z` branch does not exist). I'll do my best to respond quickly, but I can't make any guarantees until I've seen your code.

# Who can contribute

**Contributions from all sources — including humans and AI systems — are welcome.**

# Code style

- Use your own judgment and stick to the style of the surrounding code.
