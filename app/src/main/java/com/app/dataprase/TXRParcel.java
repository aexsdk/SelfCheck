package com.app.dataprase;

import android.os.Parcel;
import android.os.Parcelable;

public class TXRParcel extends TXRData implements Parcelable {

	public TXRParcel()
	{
		super();
	}
	
	public TXRParcel(TXRData data)
	{
		this.ixxleib = data.ixxleib;
		this.userID = data.userID;
		this.idianwenxs = data.idianwenxs;
		this.isendtime = data.isendtime;
		this.ccontext = data.ccontext;
	}
	
	public int describeContents() {
		// TODO Auto-generated method stub
		return 0;
	}

	public void writeToParcel(Parcel dest, int flags) {
		// TODO Auto-generated method stub
		
		dest.writeInt(ixxleib);
		dest.writeLong(userID);
		dest.writeInt(idianwenxs);
		dest.writeInt(isendtime);
		dest.writeString(ccontext);
	}
	
	public static final Parcelable.Creator<TXRParcel> CREATOR = new Creator<TXRParcel>() {

		public TXRParcel createFromParcel(Parcel source) {
			// TODO Auto-generated method stub
			
			TXRParcel data = new TXRParcel();
			
			data.ixxleib = (short)source.readInt();
			data.userID = source.readLong();
			data.idianwenxs = (short)source.readInt();
			data.isendtime = source.readInt();
			data.ccontext = source.readString();
			
			return data;
		}

		public TXRParcel[] newArray(int size) {
			// TODO Auto-generated method stub
			return new TXRParcel[size];
		}
		
	};

}
