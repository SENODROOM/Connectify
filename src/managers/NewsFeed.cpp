#include "NewsFeed.h"

void NewsFeed::generate(const User* currentUser,
                        const UserTable& allUsers,
                        FeedSnapshot& out) const {
    const FollowArray& following = currentUser->getFollowing();
    for (int f = 0; f < following.size(); ++f) {
        int followedID = following[f];
        for (int u = 0; u < allUsers.size(); ++u) {
            if (allUsers[u]->getID() == followedID) {
                PostNode* node = allUsers[u]->getPosts().head();
                while (node) { out.add(node->post); node = node->next; }
                break;
            }
        }
    }
    out.sortNewestFirst();
}

int NewsFeed::nextPostID(const UserTable& allUsers) {
    int maxID = 2000;
    for (int i = 0; i < allUsers.size(); ++i) {
        PostNode* node = allUsers[i]->getPosts().head();
        while (node) {
            if (node->post->getPostID() > maxID)
                maxID = node->post->getPostID();
            node = node->next;
        }
    }
    return maxID + 1;
}
