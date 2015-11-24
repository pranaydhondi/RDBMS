import java.io.IOException;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.StringTokenizer;
import java.util.TreeMap;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
public class NextWord {
	private static class ValueComparator implements Comparator<String> {
		java.util.Map<String,Integer> base;
	
	    public ValueComparator(java.util.Map<String,Integer> base) {
	        this.base = base;
	    }
	
	    // Note: this comparator imposes orderings that are inconsistent with
	    // equals.
	    public int compare(String a, String b) {
	        if (base.get( a) >= base.get( b)) {
	            return -1;
	        } else {
	            return 1;
	        } // returning 0 would merge keys
	    }
	}
	public static class Map extends Mapper<LongWritable, Text, Text, Text > {
		public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
			StringTokenizer itr = new StringTokenizer(value.toString()," ,-~().?_'`\"!@#$%^&*:;<>/+=|[]{}"); // line to string token
			Text word = new Text(),word1 = new Text();
			String prevword = "";
			while (itr.hasMoreTokens()) {
					String temp = itr.nextToken();
					temp = temp.toLowerCase();
					word1.set(temp);
					if(temp.length()>=3){
						for(int i=3;i<=temp.length();i++){
							word.set(temp.substring(0, i)+":");
							context.write(word, word1);
						}
					}
					if(prevword.length()>0){
						word.set(prevword+"$:");
						word1.set(temp);
						context.write(word, word1);
					}
					prevword = temp;
			}
		}
	}
	public static class Reduce extends Reducer<Text, Text, Text, Text> {

		public void reduce(Text key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
			String temp="";
			Text result = new Text();
			java.util.Map<String,Integer> m1 = new HashMap<String,Integer>();
			ValueComparator bvc = new ValueComparator(m1);
			TreeMap<String,Integer> sorted_map = new TreeMap<String,Integer>(bvc);
			Integer t;
			for (Text val : values) {
				temp = val.toString();
				if(m1.containsKey(temp)){
					t = m1.get(temp) + 1;
					m1.put(temp,t);
				}else{
					m1.put(temp, 1);
				}
			}
			sorted_map.putAll(m1);
			Iterator<Entry<String, Integer>> entries = sorted_map.entrySet().iterator();
			temp="";
			int count=0;
			/*if(key.toString().endsWith("$:")){
				while(entries.hasNext()){
					Entry<String, Integer> entry = entries.next();
					temp = temp + " "+entry.getKey()+":"+entry.getValue().toString();
				}
				
			}else{*/
			while(count<3 && entries.hasNext()){
				Entry<String, Integer> entry = entries.next();
				temp = temp + " "+entry.getKey();//+":"+entry.getValue().toString();
				count++;
			}
			//}
			result.set(temp);
			context.write(key, result); // create a pair <keyword, number of occurences>
		}
		
	}
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs(); // get all args
		if (otherArgs.length != 2) {
			System.err.println("Usage: WordCount <in> <out>");
			System.exit(2);
		}

		// 	create a job with name "wordcount"
		Job job = new Job(conf, "PrefixMatch and NextWord");
		job.setJarByClass(NextWord.class);
		job.setMapperClass(Map.class);
		job.setReducerClass(Reduce.class);

		// 	uncomment the following line to add the Combiner
		//	job.setCombinerClass(Reduce.class);


		// set output key type
		job.setOutputKeyClass(Text.class);
		// set output value type
		job.setOutputValueClass(Text.class);
		//set the HDFS path of the input data
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		// 	set the HDFS path for the output
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));

		//Wait till job completion
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	 }//end of main method
}
