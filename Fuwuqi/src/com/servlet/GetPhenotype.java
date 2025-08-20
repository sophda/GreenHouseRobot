package com.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.db.PhenoDB;

public class GetPhenotype extends HttpServlet{



    public void doGet(HttpServletRequest request,
                      HttpServletResponse response)
        throws ServletException, IOException
    {
        String username = request.getParameter("username");
        //username = new String(username.getBytes("ISO-8859-1"),"UTF-8");
        String group = request.getParameter("group");
        String phenotype = PhenoDB.getphenotype(username, group); // 株高 叶面积 茎粗 叶周长
        String zhenduanxinxi = "";
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        out.print(phenotype);
        out.flush();
        out.close();
    }
}
