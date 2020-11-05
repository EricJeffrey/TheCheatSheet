package com.thecheatsheet.cheatsheet.utils;

import com.github.kevinsawicki.http.HttpRequest;

/**
 * @Author WYB
 * @Date 2020/11/5
 **/
public class HttpUtils {


    public static HttpRequest httpGet(String url, String data) {
        return HttpRequest.get(url)
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data);
    }

    public static HttpRequest httpPut(String url, String data) {
        return HttpRequest.put(url, null, true)
                .header("Content-Type", "application/json")
                .header("Content-Length", data.getBytes().length)
                .send(data.getBytes());
    }
}
