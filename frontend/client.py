
import subprocess
from os import execl, getenv, makedirs
import sys
import os.path as path

from requests import get, post
import json
from datetime import datetime

serverHost = "202.38.86.66"
serverPort = 7080

PathGetTagList = "tagList"
PathLogin = 'login'
PathAddCodeSegment = "addCodeSegment"
PathAddTag = "addTag"
PathRegister = "Register"
PathSearch = "search"

myEmail = "1719937412@qq.com"
myPassword = "123456"

CodeSuccess = 100

OrgName = 'ericjeffrey.ustc.zzu'
AppName = 'thecheatsheet.github'


def makeUrl(path, param=None):
    res = 'http://' + serverHost + ':' + str(serverPort) + '/' + path
    if param != None:
        res += '?'
        for key in param:
            res += key + '=' + str(param[key]) + '&'
        res = res[0:len(res) - 1]
    return res


def getUserProfileFilePath():
    fileName = 'userProfile'
    appdataDir = '.'
    if sys.platform == "linux" or sys.platform == "linux2":
        appdataDir = getenv('HOME', appdataDir)
    elif sys.platform == "darwin":
        # doesnot matter
        print('for MacOS, data will be stored in current director')
        pass
    elif sys.platform == "win32":
        appdataDir = getenv('LOCALAPPDATA', appdataDir)
    path.join(appdataDir, OrgName, AppName, fileName)


def setUserInfo(userEmail, userPassword, userName=None):
    if len(userEmail) <= 0 or len(userPassword) <= 0:
        print("email and password is required")
        return
    with open(getUserProfileFilePath(), mode='w') as fp:
        data = {"email": userEmail, "password": userPassword}
        if userName != None and len(userName) > 0:
            data["name"] = userName
        fp.write(json.dumps(data))


def readUserInfo():
    if not path.exists(getUserProfileFilePath()):
        print("user profile not exist")
        return (False,)
    userProfile = json.load(open(getUserProfileFilePath(), mode='r'))
    if "email" not in userProfile or\
        "password" not in userProfile or\
        len(userProfile["emial"]) <= 0 or\
        len(userProfile["password"]) <= 0 or\
            ("name" in userProfile and userProfile["name"] <= 0):
        print("user profile broken, login or reset your user profile")
        return (False, None)
    return (True, userProfile)


def login(userEmail, userPassword):
    """ Return (bool, 'uid=xxx') """
    if len(userEmail) <= 0 or len(userPassword) <= 0:
        return (False,)
    resp = post(makeUrl(PathLogin), json={
        "email": userEmail,
        "password": userPassword
    })
    if resp.status_code != 200:
        print("request failed: %d" % (resp.status_code))
        return (False,)
    respJson = resp.json()
    if respJson["code"] != CodeSuccess:
        print("request unsuccess %d: %s" %
              (respJson["code"], respJson["errorMsg"]))
        return (False,)
    else:
        pass
    return (True, resp.headers["Set-Cookie"].split(';')[0].strip())


def register(userEmail, userPassword, userName=None):
    if len(userEmail) <= 0 or len(userPassword) <= 0:
        print("email, and password is required")
        return False
    resp = post(makeUrl(PathRegister), json={
        "email": userEmail,
        "name": userName if userName != None else "",
        "password": userPassword
    })
    if resp.status_code != 200:
        print("request failed %d" % (resp.status_code))
        return False
    respJson = resp.json()
    if respJson["code"] != CodeSuccess:
        print("request unsuccess %d %s" %
              (respJson["code"], respJson["errorMsg"]))
        return False
    return True


def search(text, pageSize=100):
    if len(text) <= 0:
        print("text is required")
        return (False, None)
    param = {"text": text, "pageSize": pageSize}
    url = makeUrl(PathSearch, param)
    resp = get(url)
    if resp.status_code == 200:
        respJson = resp.json()
        if respJson["code"] == CodeSuccess:
            return (True, respJson["result"]["codeSegments"])
        else:
            print("request unsuccess %d: %s" %
                  (respJson["code"], respJson["errorMsg"]))
    else:
        print('request failed: %d' % (resp.status_code))
    return (False, None)


def getTagList():
    resp = get(makeUrl(PathGetTagList))
    if resp.status_code != 200:
        print("request failed %d" % (resp.status_code))
        return (False, None)
    respJson = resp.json()
    if respJson["code"] != CodeSuccess:
        print("request unsuccess %d %s" %
              (respJson["code"], respJson["errorMsg"]))
        return (False, None)
    return (True, respJson["result"])


def addTag(tag, cookie):
    # todo remove redudant getTag
    if len(tag) <= 0:
        print("tag is required")
        return (False,)
    success, existedTags = getTagList()
    if not success:
        print("get tag list failed")
    else:
        for existTag in existedTags:
            if existTag['value'] == tag:
                return existTag['_id']

    resp = post(makeUrl(PathAddTag), json={
        "tag": tag
    }, headers={"Cookie": cookie})
    if resp.status_code != 200:
        print("request failed: %d" % (resp.status_code))
        return (False,)
    respJson = resp.json()
    if respJson["code"] != CodeSuccess:
        print("request unsuccess %d: %s" %
              (respJson["code"], respJson["errorMsg"]))
        return (False,)
    else:
        return (True, respJson["result"]["_id"])


def addCodeSegment(title, content, tagIdList, cookie, description=None):
    if len(title) <= 0 or len(content) <= 0 or len(tagIdList) <= 0:
        print("title, content or at least one tag is required")
        return (False, None)
    resp = post(makeUrl(PathAddCodeSegment), json={
        "codeSegment": {
            "title": title,
            "content": content,
            "description": "" if description == None else description,
            "createdAt": (int)(datetime.now().timestamp() * 1000),
            "lastModified": (int)(datetime.now().timestamp() * 1000),
            "favorNumber": 0,
            "tagList": tagIdList
        }
    }, headers={"Cookie": cookie})
    if resp.status_code != 200:
        print("request failed %d" % (resp.status_code))
        return (False, None)
    respJson = resp.json()
    if respJson["code"] != CodeSuccess:
        print("request unsuccess %d: %s" %
              (respJson["code"], respJson["errorMsg"]))
        return (False, None)
    return (True, respJson["result"]["_id"])


def main():
    help = '''TheCheatsheet - https://github.com/EricJeffrey/TheCheatSheet
subcommand:
    add         add a code segment
    search      search any code segment
'''
    argv = sys.argv
    argc = len(argv)
    if argc == 1 or argc > 2:
        print(help)
    else:
        cmd = argv[1]
        if cmd == "add":
            tmp = login(myEmail, myPassword)
            if tmp[0]:
                cookie = tmp[1]
                print("title (one line): ", end='', flush=True)
                title = sys.stdin.readline().strip()
                print("description (optioanl, one line): ", end='', flush=True)
                desc = sys.stdin.readline().strip()
                exitKey = "Ctrl+Z" if sys.platform == "win32" else "Ctrl+D"
                print("content (multi lines, use " + exitKey +
                      " on a NEW LINE to end input): ",  flush=True)
                content = sys.stdin.read()
                print("tags (comma seperated, i.e. linux,java ): ",
                      end='', flush=True)
                tags = sys.stdin.readline().strip()
                tagList = [x.strip() for x in tags.split(',')]
                tagIdList = []
                for tag in tagList:
                    tmp = addTag(tag, cookie)
                    if tmp[0]:
                        tagIdList.append(tmp[1])
                    else:
                        print("＞﹏＜ add tag failed")
                        return
                success, id = addCodeSegment(
                    title, content, tagIdList, cookie, desc)
                if success:
                    print("added: %s" % (id))
                else:
                    print("＞﹏＜ add code segment failed")
            else:
                print("＞﹏＜ login failed, bye")
                return
        elif cmd == "search":
            # todo should not be needed
            success, tagList = getTagList()
            if success:
                tagDict = {}
                for tag in tagList:
                    tagDict[tag['_id']] = tag['value']
                text = input("text: ").strip()
                success, segments = search(text)
                if success:
                    out = "---------------------------------------------\n\n" if len(
                        segments) > 0 else "Nothing Found ~(￣▽￣)~*"
                    for segment in segments:
                        out += "Title: " + segment["title"]
                        out += "\n"
                        out += "Desc: " + segment["description"]
                        out += "\n"
                        out += "Code:\n" + segment["content"]
                        out += "\n"
                        out += "Tags: " + \
                            ','.join([tagDict[v] for v in segment['tagList'] if v in tagDict])
                        out += "\n---------------------------------------------\n\n"
                    out += "No More (●'◡'●)" if len(segments) > 0 else ""
                    fileName = 'segments.tmp.txt'
                    fileDirPath = ''
                    print("\033[2J\033[1;1H")
                    if sys.platform == "win32":
                        fileDirPath = path.join(
                            getenv("LOCALAPPDATA", '.'), OrgName, AppName)
                        makedirs(fileDirPath, exist_ok=True)
                        with open(path.join(fileDirPath, fileName), mode='w', encoding='GB18030') as fp:
                            fp.write(str(out.encode('GB18030'), 'GB18030'))
                        subprocess.call(
                            ["C:\\Windows\\System32\\more.com", path.join(fileDirPath, fileName)])
                    else:
                        fileDirPath = path.join('/tmp', OrgName, AppName)
                        makedirs(fileDirPath, exist_ok=True)
                        with open(path.join(fileDirPath, fileName), mode='w', encoding='utf-8') as fp:
                            fp.write(out)
                        subprocess.call(
                            ['/usr/bin/more', path.join(fileDirPath, fileName)])
                else:
                    print("＞﹏＜ search failed")
            else:
                print("＞﹏＜ get tag list failed")
        else:
            print(help)


main()
