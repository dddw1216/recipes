#!/bin/sh

# 配置新的作者信息
OLD_AUTHOR_NAME="Old Author Name"
NEW_AUTHOR_NAME="New Author Name"
NEW_AUTHOR_EMAIL="new.author@email.com"

# 临时备份分支名称
BACKUP_BRANCH_NAME="backup-$(date +%s)"

# 创建一个备份分支
git branch "$BACKUP_BRANCH_NAME"

# 运行过滤分支命令
git filter-branch -f --env-filter "
if [ \"\$GIT_AUTHOR_NAME\" = '$OLD_AUTHOR_NAME' ]
then
    export GIT_AUTHOR_NAME='$NEW_AUTHOR_NAME'
    export GIT_AUTHOR_EMAIL='$NEW_AUTHOR_EMAIL'
fi
if [ \"\$GIT_COMMITTER_NAME\" = '$OLD_AUTHOR_NAME' ]
then
    export GIT_COMMITTER_NAME='$NEW_AUTHOR_NAME'
    export GIT_COMMITTER_EMAIL='$NEW_AUTHOR_EMAIL'
fi
" -- --all
