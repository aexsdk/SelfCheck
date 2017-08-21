package com.hthx.tpse01.selfcheck.hardware;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.IOException;

import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.widget.Toast;

public class HardwareCheck {
	
	public boolean getTFCardState(){                         //TF卡状态
		
		File dev = new File("/dev");
		File[] files = dev.listFiles();
		int i;
		for (i = 0; i < files.length; i++) {
			if (files[i].getName().equals("jw_tf")) {
				return true;
			}
		}
		
		return false;
	}
	
	public String getRAMSize(){                             //运行内存大小
		
		String dir = "/proc/meminfo";
		
		double dSize = 0.0;        //0.0表示内存异常
		
		try{
			FileReader fr = new FileReader(dir);
			BufferedReader br = new BufferedReader(fr,2048);
			String memoryLine = br.readLine();
			String subMemoryLine = memoryLine.substring(memoryLine.indexOf("MemTotal:"));
			br.close();
			
			dSize = Integer.parseInt(subMemoryLine.replaceAll("\\D+",""))/(1024.0 * 1024);
			
		}catch (IOException e){
			e.printStackTrace();
		}
		
		String sSize = String.format("%.2fGB", dSize);
		
		return sSize;         
		
	}
	
	public String getROMSize(){                           //机身存储大小
		
		double dSize = 0.0;        //0.0表示内存异常
		
		if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){
			File sdCardDir = Environment.getExternalStorageDirectory();
			StatFs sf = new StatFs(sdCardDir.getPath());
			
			long bSize = sf.getBlockSize();
			long bCount = sf.getBlockCount();
			
			dSize = (bSize * bCount)/(1024.0 * 1024 * 1024);
			
		}
		
		String sSize = String.format("%.2fGB", dSize);
		
		return sSize;             
		
	}
	
	public String getOSVersion(){                          //获取系统版本
		
		//String osVersion = System.getProperty("os.version"); //获取对应linux版本
		String osVersion = Build.VERSION.RELEASE;
		
		return osVersion;
		
	}
	
	public int getNumberOfCPUCores(){                      //获取CPU核心数
		
		if(Build.VERSION.SDK_INT <= Build.VERSION_CODES.GINGERBREAD_MR1){    //android2.3.3之前的是一个核心数
			return 1;
		}
		
		int cores;
		
		try{
			cores = new File("/sys/devices/system/cpu").listFiles(filter).length;
		}catch (SecurityException e){
			cores = 0;                          //设备信息获取不到；
		}catch (NullPointerException e){
			cores = 0;                          //设备信息获取不到；
		}
		
		return cores;
	}
	
	public FileFilter filter = new FileFilter(){

		@Override
		public boolean accept(File pathname) {
			// TODO Auto-generated method stub
			
			String path = pathname.getName();
			if(path.startsWith("cpu")){
				for(int i = 3; i < path.length(); i++){
					if(path.charAt(i) < '0' || path.charAt(i) > '9'){
						return false;
					}
				}
				return true;
			}

			return false;
		}
		
	};
	
}
