#include <bits/stdc++.h>
using namespace std;
int lengthOfLongestSubstring(string s)
{
    int count = 0, result = 0, vtEnd = -1;
    unordered_map<int, int> maps;
    int maxLength = s.size();
    for (int i = 0; i < maxLength; ++i)
    {
        auto it = maps.find(s[i]);
        if (it == maps.end())
        {
            maps.insert({s[i], i});
            ++count;
        }
        else
        {
            count = i - max(vtEnd, maps[s[i]]);
            vtEnd = max(vtEnd, maps[s[i]]);
            maps[s[i]] = i;
        }
        result = max(count, result);
        // cout << s[i] << ": " << i << " " << count << ". VTend: " << vtEnd << "\n ";
    }
    return result;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    // int n, target;
    // cin >> n >> target;
    // vector<int> nums(n);
    // for (int i = 0; i < n; ++i)
    //     cin >> nums[i];

    // auto ok = twoSum(nums, target);
    // cout << ok[0] << " and " << ok[1];
    cout << lengthOfLongestSubstring("abcabcbb");
    cout << lengthOfLongestSubstring("bbbbb");
    cout << lengthOfLongestSubstring("pwwkew");

    // cout << lengthOfLongestSubstring("abcabcda");
    // cout << lengthOfLongestSubstring("abcdegh");
    cout << lengthOfLongestSubstring("cdcda");

    return 0;
}
