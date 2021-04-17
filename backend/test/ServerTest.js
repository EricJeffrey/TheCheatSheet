var tester = {
    testRespList: {},
    startTetst: async function () {
        var randStr = function (length = 8) {
            var result = [];
            var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
            // characters += '0123456789';
            var charactersLength = characters.length;
            for (var i = 0; i < length; i++) {
                result.push(characters.charAt(Math.floor(Math.random() *
                    charactersLength)));
            }
            return result.join('');
        };
        var sleep = async function (ms) { return new Promise(resolve => setTimeout(resolve, ms)); }

        const PATH_GET_CODE_SEGMENTS = "/getCodeSegments";
        const PATH_ADD_CODE_SEGMENT = "/addCodeSegment";
        const PATH_UPDATE_CODE_SEGMENT = "/updateCodeSegment";
        const PATH_SEARCH_CODE_SEGMENT = "/search";
        const PATH_ADD_TAG = "/addTag";
        const PATH_GET_TAG_LIST = "/tagList";
        const PATH_FAVOR = "/favor";
        const PATH_LOGIN = "/login";
        const PATH_REGISTER = "/register";
        const PATH_GET_USER_FAVORS = "/userFavors";
        const PATH_GET_USER_FAVOR_IDS = "/userFavorIds";
        const PATH_GET_USER_PROFILE = "/userProfile";

        const doGet = 0, doPost = 1;

        var testNext = async function (resKey, path, get0Post1, bodyOrParam, addToList = true, logout = true) {
            var res = {
                "ok": true,
                "status": 200,
                "msg": "success",
                "testNext": testNext
            };
            var config = { method: get0Post1 === doGet ? "GET" : "POST" };
            if (get0Post1 === doGet && bodyOrParam) {
                path += "?";
                for (const key in bodyOrParam) {
                    path += key + "=" + bodyOrParam[key] + "&";
                }
                path = path.substr(0, path.length - 1);
            } else if (get0Post1 === doPost && bodyOrParam) {
                config["body"] = JSON.stringify(bodyOrParam);
            }
            var resp = await fetch(path, config);
            res.msg = await resp.json();
            if (!resp.ok) {
                res.ok = false;
                res.status = resp.status;
            }
            if (logout)
                console.log(resKey, res.msg);
            if (addToList)
                tester.testRespList[resKey] = res;
            return res;
        }

        var testUserEmail = "abc" + randStr();

        await testNext(1, PATH_GET_CODE_SEGMENTS, doGet, { page: 1, pageSize: 10 });
        await testNext(2, PATH_GET_CODE_SEGMENTS, doGet, { page: 1, pageSize: 10, sortOrder: "favorNumber" });
        await testNext(3, PATH_GET_TAG_LIST, doGet, null);
        await testNext(4, PATH_GET_USER_PROFILE, doGet, null);
        await testNext(5, PATH_SEARCH_CODE_SEGMENT, doGet, { text: "html", page: 1, pageSize: 12 });
        await testNext(6, PATH_REGISTER, doPost, { email: testUserEmail, name: "jeffrey", password: "abc" });
        await testNext(7, PATH_LOGIN, doPost, { email: testUserEmail, password: "abc" });
        // sleep for a while
        // await sleep(200);
        await testNext(8, PATH_GET_USER_PROFILE, doGet, null);
        await testNext(8.1, PATH_ADD_TAG, doPost, { "tag": "taggg" + randStr() });
        await testNext(8.2, PATH_ADD_TAG, doPost, { "tag": "taggg" + randStr() });
        await testNext(8.3, PATH_ADD_TAG, doPost, { "tag": "taggg" + randStr() });
        await testNext(9, PATH_ADD_CODE_SEGMENT, doPost, {
            codeSegment: {
                title: "ttttttttttttttttti " + randStr(),
                description: "desc and dance " + randStr(),
                content: "hello something just like this",
                createdAt: 1618205862167,
                lastModified: 1618205862167,
                favorNumber: 344,
                tagList: ["python", "java", "nothing"],
            }
        });
        for (let i = 1; i < 23; i++) {
            const ii = i;
            await testNext(9 + i * 0.01, PATH_ADD_CODE_SEGMENT, doPost, {
                codeSegment: {
                    title: " actuall with title" + randStr() + "--" + ii,
                    description: "description with " + randStr(),
                    content: "night mare, night song, song of the night, sound of silence",
                    createdAt: 1618205862167,
                    lastModified: 1618205862167,
                    favorNumber: 23,
                    tagList: ["Python", randStr(3), randStr(5), "win", "C++", "sdlfk"],
                }
            });
        }
        await testNext(10, PATH_GET_CODE_SEGMENTS, doGet, { page: 1, pageSize: 3 });
        await testNext(11, PATH_GET_CODE_SEGMENTS, doGet, { page: 3, pageSize: 6 });
        await testNext(12, PATH_GET_USER_FAVOR_IDS, doGet, null);
        await testNext(13, PATH_GET_USER_FAVORS, doGet, { page: 1, pageSize: 20 });
        await testNext(14, PATH_FAVOR, doPost, { codeSegmentId: tester.testRespList[10].msg.result.codeSegments[0]._id });
        await testNext(15, PATH_FAVOR, doPost, { codeSegmentId: tester.testRespList[10].msg.result.codeSegments[1]._id });
        await testNext(16, PATH_GET_USER_FAVORS, doGet, { page: 1, pageSize: 20 });
        await testNext(17, PATH_GET_USER_FAVORS, doGet, { page: 1, pageSize: 20 });
        await testNext(18, PATH_ADD_CODE_SEGMENT, doPost, {
            codeSegment: {
                title: "标题 now we got another two bad-luck guy" + randStr(),
                description: "well dance cells!" + randStr(),
                content: "offset + limit to pagination",
                createdAt: 1618205012167,
                lastModified: 1618205872167,
                favorNumber: 344,
                tagList: [],
            }
        });
        await testNext(19, PATH_ADD_TAG, doPost, { "tag": "addedtag" });
        await testNext(20, PATH_GET_TAG_LIST, doGet, null);
        await testNext(21, PATH_UPDATE_CODE_SEGMENT, doPost, {
            "codeSegment": {
                _id: this.testRespList[10].msg.result.codeSegments[0]._id,
                esId: this.testRespList[10].msg.result.codeSegments[0].esId,
                title: "标题 now we got another two bad-luck guy ----- updated" + randStr(),
                description: "well dance cells! - updated",
                content: "UPDATED --------- offset + limit to pagination",
                createdAt: 1618205712167,
                lastModified: 1618205972167,
                favorNumber: 844,
                tagList: ["win", "JAVA", "HTML"],
            }
        });
        await testNext(22, PATH_GET_TAG_LIST, doGet);
        // sleep a while for es to index
        await sleep(200);
        await testNext(22, PATH_SEARCH_CODE_SEGMENT, doGet, { text: "dance", page: 1, pageSize: 33 });
        await testNext(24, PATH_GET_USER_FAVOR_IDS, doGet, null);
        return 0;
    }
};


tester.startTetst().then(v => console.log("DONE"));