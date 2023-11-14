Competition Template
---

Typical competition program


```
void pre_auton( void );
void autonomous( void );
void usercontrol( void );
```


Steps to use GitHub:
Step 1(Sometimes unnecessary(in this case it is)): Go to github.com, sign into your account, go the the Repositories tab, click new, create a new repository, and copy the repo link (will look something like this: github.com/username/repo-name)
Step 2: In terminal, change directories to the own you are going to create a repo in by typing the command "cd your/file/path"
Step 3: Initiate the repository by typing "git init"
Step 4: Type "git status" to confirm the repo is working properly
Step 5: If the project is small, run the command "git add ." to add all files to the repo, if its a larger project however, either run "git add filename.filetype" for large files or if you want to add all files of a file type, run "git add *.filetype"
Step 6: Run "git status" again to confirm everything has been added
Step 7: After making changes to code or the project, run the command "git commit -m "message" " to commit the project to the new local repo.
Step 8(Sometimes unecessary): If you want to make the repository remote so it can be accessed by multiple devices, run the command "git remote add origin github.com/username/repo-name" to connect the local repo with the remote one.
Step 9: When you are ready to push the local code to the remote repo, run the command "git push". Sometimes you'll get an error that says something along the lines of it not being the upstream branch, if so, run the command "git push --set-upstream origin master" to push it.
Step 10: To commit and push new changes, follow steps 5-7, and then step 9.
Step 11: If you're collaborating with other users, just ask Chase for help, he's too lazy to write more steps.