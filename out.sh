#!/bin/bash

# Step 1: Remove the large file from the repository
large_file="project_code/intra/website/out"

echo "Removing the large file: $large_file"
git rm "$large_file"

# Step 2: Commit the removal
commit_message="Remove large file $large_file"
echo "Committing the removal with message: $commit_message"
git commit -m "$commit_message"

# Step 3: Push the change to the remote repository
branch="scale_send"
echo "Pushing the changes to the remote branch: $branch"
git push origin "$branch"

# Step 4: Optionally, modify .gitattributes to stop tracking the large file with Git LFS
# Uncomment the following lines if you want to edit .gitattributes to stop tracking the file:
# git lfs track --overwrite "$large_file"
# git add .gitattributes
# git commit -m "Stop tracking large file $large_file with Git LFS"
# git push origin "$branch"

echo "Done!"

