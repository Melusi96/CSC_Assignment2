//Creating an AVL Tree app
//Melusi Gumbi
//12 April 2021

import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

public class AccessAVLApp{
   //Create a static tree for easy access around the methods created
   static AVLTree<Student> tree = new AVLTree<Student>();
   
   public static void main(String[] args){
      try{
         //Create the file
         Scanner file = new Scanner(new File("oklist.txt"));
         
         /*Check if there are still lines in the list
          and insert the student's details to the tree*/
         while (file.hasNextLine()){
            tree.insert(new Student(file.next(), file.next(), file.next()));
         }
      }
      catch (Exception e){
         System.out.println(e);
      }
      
      //checking for any arguments in preparation for an output
      if (args.length == 0){
         printAllStudents();
      }
      else if (args.length == 1){
         printStudent(args[0]);
      }
   }
   //Create the print student method
   public static void printStudent(String num){
   
      BinaryTreeNode<Student> node = tree.find(new Student(num));
      if (node == null){
         System.out.println("Access denied!");
      }
      else{
         tree.visit(node);
      }
   }
   
   //Create a method to printout the whole oklist
   public static void printAllStudents(){
      tree.inOrder();
   }
}