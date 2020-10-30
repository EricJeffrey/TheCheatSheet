import axios from 'axios';

const BASE_URL = "/codeSegment/";
const DEFAULT_PAGE_SIZE = 20;

const MOCK_CODE_SEGMENTS = [
    {
        "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
        "title": "Linux创建空文件",
        "description": "",
        "code": "touch a.txt",
        "tag": "Command",
        "createAt": "1603092591383",
        "lastModify": "1603092601384"
    },
    {
        "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
        "title": "Linux创建空文件",
        "description": "",
        "code": "touch a.txt",
        "tag": "Command",
        "createAt": "1603092591383",
        "lastModify": "1603092601384"
    },
    {
        "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
        "title": "Linux创建空文件",
        "description": "",
        "code": "touch a.txt",
        "tag": "Command",
        "createAt": "1603092591383",
        "lastModify": "1603092601384"
    }
];

const client = {
    getCodeSegments: function (pageCnt) {
        return new Promise((resolve, reject) => {
            resolve(MOCK_CODE_SEGMENTS);
        });
    },
    getCodeSegmentsByTag: function (tag, pageCnt) {
        return new Promise((resolve, reject) => {
            resolve([{
                "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
                "title": "Linux创建空文件",
                "description": "",
                "code": "touch a.txt",
                "tag": "Command",
                "createAt": "1603092591383",
                "lastModify": "1603092601384"
            }]);
        });
    },
    getAllTags: function () {
        return new Promise((resolve, reject) => {
            resolve(["Java", "C++", "Python"]);
        });
    },
    searchCodeSegments: function (text, pageCnt) {
        return new Promise((resolve, reject) => {
            resolve(MOCK_CODE_SEGMENTS.slice(0, 2));
        });
    },
    updateCodeSegment: function (codeSegment) {
        return new Promise((resolve, reject) => {
            resolve();
        });
    },
    addCodeSegment: function (title, description, tag, code) {
        return new Promise((resolve, reject) => {
            resolve();
        });
    }
};
export default client;