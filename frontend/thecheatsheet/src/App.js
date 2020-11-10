import React, { Component } from 'react';
import { Button, Card, Input, Layout, Menu, message, Select, Space } from 'antd';
import SyntaxHighlighter from 'react-syntax-highlighter';
import {
    EyeOutlined, EditOutlined,
    CloseCircleOutlined, CheckCircleOutlined, DownOutlined
} from '@ant-design/icons';
import 'antd/dist/antd.css';
import './css/App.css';
import Modal from 'antd/lib/modal/Modal';
import TextArea from 'antd/lib/input/TextArea';
import { docco } from 'react-syntax-highlighter/dist/esm/styles/hljs';
import client from './client';
import Text from 'antd/lib/typography/Text';

const { Header, Sider, Content } = Layout;

const DATA_TYPE_ALL = 1;
const DATA_TYPE_BY_TAG = 2;
const DATA_TYPE_SEARCH = 3;

const ERROR_MESSAGE_DATA = "获取数据失败";

// FIXME tagList can not update - elasticsearch not responding very quickly
export default class App extends Component {
    constructor(props) {
        super(props);

        this.dataType = DATA_TYPE_ALL;
        this.pageCount = 0;
        this.searchText = null;
        this.state = {
            dialogStatus: {
                visible: false,
                editable: false,
                adding: false,
                data: null,
                targetIndex: null
            },
            tagList: [],
            data: [],
            menuSelectedKey: "0",
        };

        this.onMenuClick = this.onMenuClick.bind(this);
        this.viewDetail = this.viewDetail.bind(this);
        this.onSearchPressEnter = this.onSearchPressEnter.bind(this);
        this.onAddClick = this.onAddClick.bind(this);
        this.updateType = this.updateType.bind(this);
        this.loadMore = this.loadMore.bind(this);
        this.updateAfterModal = this.updateAfterModal.bind(this);
    }

    updateType(type, pageCount, searchText) {
        this.pageCount = pageCount;
        this.dataType = type;
        if (searchText != null)
            this.searchText = searchText;
    }

    loadMore() {
        var tmpPromise;
        switch (this.dataType) {
            case DATA_TYPE_ALL:
                tmpPromise = client.getCodeSegments(this.pageCount + 1);
                break;
            case DATA_TYPE_BY_TAG:
                const i = Number.parseInt(this.state.menuSelectedKey);
                tmpPromise = client.getCodeSegmentsByTag(this.state.tagList[i], this.pageCount + 1);
                break;
            case DATA_TYPE_SEARCH:
                tmpPromise = client.searchCodeSegments(this.searchText, this.pageCount + 1);
                break;
            default:
                return;
        }
        tmpPromise.then((codeSegments) => {
            this.updateType(this.dataType, this.pageCount + 1);
            this.setState({
                data: this.state.data.concat(codeSegments)
            });
        }).catch(() => { message.error(ERROR_MESSAGE_DATA); });
    }

    componentDidMount() {
        client.getAllTags()
            .then((tagList) => {
                this.setState({ tagList: ["All", ...tagList] });
                client.getCodeSegments(0)
                    .then((codeSegments) => {
                        this.updateType(DATA_TYPE_ALL, 0);
                        this.setState({ data: codeSegments });
                    })
                    .catch(() => { message.error(ERROR_MESSAGE_DATA) });
            })
            .catch(() => {
                message.error("获取标签列表失败");
            });
    }

    onSearchPressEnter(e) {
        const txt = e.target.value;
        if (txt == null || txt.length === 0) return;
        client.searchCodeSegments(txt, 0)
            .then((codeSegments) => {
                this.updateType(DATA_TYPE_SEARCH, 0, txt);
                this.setState({ data: codeSegments });
            })
            .catch(() => {
                message.error(ERROR_MESSAGE_DATA);
            });
    }

    onAddClick(e) {
        this.setState({
            dialogStatus: {
                visible: true,
                adding: true,
                editable: true,
                data: {
                    id: "", title: "", description: "", code: "",
                    tag: "", createAt: "", lastModify: ""
                }
            }
        });
    }

    onMenuClick({ item, key, keyPath, domEvent }) {
        var tmpPromise = (key === "0")
            ? client.getCodeSegments(0)
            : client.getCodeSegmentsByTag(this.state.tagList[Number.parseInt(key)], 0);
        tmpPromise.then((codeSegments) => {
            this.updateType((key === "0") ? DATA_TYPE_ALL : DATA_TYPE_BY_TAG, 0);
            this.setState({ menuSelectedKey: key, data: codeSegments });
        }).catch(() => {
            message.error(ERROR_MESSAGE_DATA);
        });
    }

    viewDetail(index, doEdit) {
        this.setState({
            dialogStatus: {
                visible: true,
                adding: false,
                editable: doEdit,
                data: this.state.data[index],
                targetIndex: index
            }
        });
    }

    updateAfterModal(addCanceled, index, newData) {

        if (this.state.dialogStatus.adding) {
            if (addCanceled === true)
                return;
            window.location.reload();
            // client.getAllTags()
            //     .then((tagList) => {
            //         this.setState({ tagList: ["All", ...tagList] });
            //         if (this.dataType === DATA_TYPE_ALL) {
            //             client.getCodeSegments(0)
            //                 .then((codeSegments) => {
            //                     this.updateType(DATA_TYPE_ALL, 0);
            //                     this.setState({ data: codeSegments });
            //                 })
            //                 .catch(() => { message.error(ERROR_MESSAGE_DATA); })
            //         }

            //     })
            //     .catch((r) => { message.error("获取标签列表失败"); });
        }
        // update
        if (index !== null) {
            client.getAllTags().then((tagList) => {
                this.setState((prevState) => {
                    var newState = prevState;
                    newState.data[index] = newData;
                    newState.tagList = ["All", ...tagList]
                    return newState;
                });
            }).catch(() => { message.error("获取标签列表失败"); });
        }
    }

    render() {
        const menuItems = <>
            {this.state.tagList.map((value, index) =>
                <Menu.Item key={index.toString(10)}>{value}</Menu.Item>
            )}
        </>;
        const cardLists = <>
            {this.state.data.map((value, index) => {
                var descDiv;
                if (value.description === null || value.description.length === 0)
                    descDiv = <Text style={{ width: "100%" }} ellipsis={true} >{value.title}</Text>;
                else {
                    descDiv = <div>{
                        value.description.split('\n').map(
                            (tmpv, index) => <Text style={{ width: "100%" }} ellipsis={true} key={index}>{tmpv}</Text>
                        )}
                    </div>;
                }
                return <Card className="main_card"
                    title={<Text ellipsis={true} style={{ fontWeight: "bold", width: "100%", marginRight: 10 }}>{value.title}</Text>}
                    key={index}
                    extra={
                        <Space>
                            <EyeOutlined onClick={() => { this.viewDetail(index, false); }} />
                            <EditOutlined onClick={() => { this.viewDetail(index, true); }} />
                        </Space>
                    }>
                    {descDiv}
                    <Text ellipsis={true} copyable={true}>{value.code}</Text>
                </Card>
            })}
        </>;

        return (
            <Layout className="comon_theme" id="main">
                <Header className="comon_theme" id="main_header">
                    <a href="/" id="main_logo">TheCheatsheet</a>
                    <div id="main_search_holder">
                        <Input
                            id="main_input_search"
                            onPressEnter={this.onSearchPressEnter}
                            placeholder="search" />
                        <div className="middle_space"></div>
                        <Button
                            id="main_btn_add"
                            onClick={this.onAddClick} >添加</Button>
                    </div>
                </Header>
                <Layout style={{ width: "100%" }}>
                    <Sider collapsible={true} className="comon_theme" id="main_sider" theme="light">
                        <Menu
                            id="main_menu"
                            onClick={this.onMenuClick}
                            selectedKeys={this.state.menuSelectedKey}>
                            {menuItems}
                        </Menu>
                    </Sider>
                    <Content className="comon_theme" id="main_content">
                        <div className="comon_theme" id="main_card_holder">
                            {cardLists}
                        </div>
                        <Button
                            onClick={(e) => { this.loadMore(); }}
                            icon={<DownOutlined />}
                            style={{ border: "none", background: "#ffffff33" }}>加载更多</Button>
                    </Content>
                    <DetailModal
                        updateAfterModal={this.updateAfterModal}
                        status={this.state.dialogStatus} />
                </Layout>
            </Layout>
        );
    }
};

class DetailModal extends Component {
    constructor(props) {
        super(props);

        this.supportedTagList = ["Text", "Linux", "C++", "Java", "Python", "Bash", "JavaScript", "HTML", "CSS",
            "RegularExpr", "Android", "Windows", "Cmd", "Go", "C#", "Kotlin", "TypeScript", "Dart", "Php", "Rust"];
        this.status = {
            visible: false,
            editable: false,
            adding: false,
            addResult: false,
            data: {
                title: null,
                description: null,
                code: null,
                tag: null,
            },
            targetIndex: null,
            lastSuccessJobData: null
        };
        this.updateAfterModal = props.updateAfterModal;

        this.closeMe = this.closeMe.bind(this);
        this.onDoEdit = this.onDoEdit.bind(this);
        this.onTagTypeChange = this.onTagTypeChange.bind(this);
        this.update = this.update.bind(this);
        this.onDescChange = this.onDescChange.bind(this);
        this.onCodeChange = this.onCodeChange.bind(this);
        this.onTitleChange = this.onTitleChange.bind(this);
    }
    onTagTypeChange(value, option) {
        this.status.data.tag = value;
        this.setState({});
    }

    onDoEdit(e) {
        if (!this.status.editable) {
            this.status.editable = true;
            this.setState({});
        }
    }

    update(e, closeAfterupdate) {
        function isEmpty(s) {
            return s === null || s === undefined || s === "";
        }
        var tmpPromise;
        if (this.status.adding) {
            const data = this.status.data;
            if (isEmpty(data.title) && isEmpty(data.description)) {
                message.warn("标题或描述不可为空");
                return;
            }
            else if (isEmpty(data.code)) {
                message.warn("代码不可为空");
                return;
            }
            else
                tmpPromise = client.addCodeSegment(data.title, data.description, data.tag, data.code);
        } else {
            tmpPromise = client.updateCodeSegment(this.status.data);
        }
        tmpPromise.then(() => {
            this.status.editable = false;
            this.status.lastSuccessJobData = Object.assign({}, this.status.data);
            this.status.addResult = true;
            message.info((this.status.adding ? "添加" : "更新") + "成功")
            this.setState({});
        }).catch(() => {
            message.error((this.status.adding ? "添加" : "更新") + "失败");
        }).then(() => {
            if (closeAfterupdate != null && closeAfterupdate === true)
                this.closeMe();
        });
    }

    closeMe() {
        this.status.visible = false;
        this.setState({});
    }

    onDescChange(e) {
        this.status.data.description = e.target.value;
        this.setState({});
    }

    onTitleChange(e,) {
        this.status.data.title = e.target.value;
        this.setState({});
    }

    onCodeChange(e) {
        this.status.data.code = e.target.value;
        this.setState({});
    }

    render() {
        this.status = this.props.status;
        this.status.lastSuccessJobData = Object.assign({}, this.status.data);

        if (this.status.adding && this.status.data.tag === "")
            this.status.data.tag = this.tagList[0];

        if (this.status.data === null)
            return <div></div>;
        const disabled = !this.status.editable;

        // rawTag is for [Select]
        var rawTag = "";
        if (this.status.data !== null)
            rawTag = this.status.data.tag;
        if (rawTag === null || rawTag.length === 0) rawTag = this.tagList[0];

        // tag is for SyntaxHightlight
        var language = rawTag;
        language = language.toLowerCase();
        if (language === "c++") language = "cpp";
        else if (language === "c#") language = "csharp";


        return <Modal
            afterClose={() => {
                // when is cancel
                this.updateAfterModal(
                    !(this.status.addResult === true), this.status.targetIndex, this.status.lastSuccessJobData);
            }}
            onOk={(e) => { this.update(e, true) }}
            onCancel={this.closeMe}
            className="main_modal"
            visible={this.status.visible}
            closable={false}>
            <div className="main_modal_content">
                <Space style={{
                    width: "100%",
                    textAlign: "end",
                    display: "inline-flex",
                    justifyContent: "flex-end",
                    marginBottom: "1em"
                }}>
                    <CheckCircleOutlined hidden={disabled || this.status.adding} onClick={this.update} />
                    <EditOutlined onClick={this.onDoEdit} />
                    <CloseCircleOutlined onClick={this.closeMe} />
                </Space>
                <div className="comon_theme"
                    style={{ marginBottom: "1em" }}>
                    <Input
                        disabled={disabled}
                        placeholder="标题"
                        onChange={this.onTitleChange}
                        value={this.status.data.title} />
                    <div className="small_space" />
                    <Select
                        style={{ minWidth: "7em" }}
                        disabled={disabled}
                        value={rawTag}
                        onChange={this.onTagTypeChange}>
                        {this.tagList.map((value, index) =>
                            <Select.Option value={value} key={index}>
                                {value}
                            </Select.Option>)}
                    </Select>
                </div>
                <TextArea
                    disabled={disabled}
                    placeholder="代码段描述"
                    onChange={this.onDescChange}
                    style={{ marginBottom: "1em" }}
                    value={this.status.data.description} />
                <div
                    className="common_theme main_modal_code"
                    style={{ marginBottom: "1em" }}>
                    <TextArea
                        placeholder="代码"
                        hidden={disabled}
                        value={this.status.data.code}
                        onChange={this.onCodeChange}
                        className="main_modal_code_editor" />
                    <SyntaxHighlighter
                        className="main_modal_highlighter"
                        style={docco}
                        language={language}
                        showLineNumbers>{this.status.data.code}
                    </SyntaxHighlighter>
                </div>
            </div>
        </Modal>
    }
}
