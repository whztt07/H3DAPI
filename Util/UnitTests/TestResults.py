﻿import os
from collections import namedtuple
import glob
#import Image
import subprocess
import difflib
import math

class TestResults ( object ):
  # Container class for all kinds of possible test results. Also processes and validates the results
  # The get functions are used to validate results and will return a tuple that should be added to the array that is passed to addStepResults

  StepResultTuple = namedtuple('StepResult', ['step_name', 'success', 'results']) # results is an array of results
  PerformanceResultTuple = namedtuple("PerformanceResult", ['fps_min', 'fps_max', 'fps_avg', 'fps_mean', 'fps_full'])
  RenderingResultTuple = namedtuple("RenderingResult", ['success', 'baseline_path','output_path', 'diff_path'])
  ConsoleResultTuple = namedtuple("ConsoleResult", ['success', 'baseline_path', 'output', 'diff'])
  CustomResultTuple = namedtuple("CustomResult", ['success', 'baseline_path', 'output', 'diff'])
  ErrorResultTuple = namedtuple("ErrorResult", ['stdout', 'stderr'])

  def __init__ ( self ):
    self.filename= ""
    self.case_name= ""
    self.result_type= ""
    self.url= ""
    self.std_out= ""
    self.std_err= ""
    self.starts_ok= False
    self.terminates_ok= False
  
    self.step_list = []
    
    self.warnings= 0
    self.errors= 0
    self.skipped= False

  def addStepResults(step_name, success, results):
    self.step_results.append(self.StepResult(step_name, success, results))

  def getPerformanceResult(self, fps_data):
      fps_list = fps_data.split()
      fps_list_float = []
      for fps in fps_list:
        fps_list_float.append( float( fps ) )
      if len( fps_list_float ) > 0:
        fps_list_float.sort()
        return TestResults.PerformanceResultTuple("%.2f" % fps_list_float[0], "%.2f" % fps_list_float[len( fps_list_float ) - 1], "%.2f" % (math.fsum( fps_list_float ) / float( len( fps_list_float ) ) ), "%.2f" % float( fps_list_float[len( fps_list_float ) / 2] ), fps_data)
  
  def getRenderingResult(self, baseline_path, output_path):
    valid, diff_path, error = self._validate_rendering(baseline_path.replace('\\', '/'), output_path.replace('\\', '/'))
    for line in error:
      print line
    return TestResults.RenderingResultTuple(valid, baseline_path, output_path, diff_path)      
  
  def getCustomResult(self, baseline_path, output):
    valid, diff, error = self._validate_text(baseline_path, output)
    for line in error:
      print line
    return TestResults.CustomResultTuple(valid, baseline_path, output, diff)

  def getConsoleResult(self, baseline_path, output):
    valid, diff, error = self._validate_text(baseline_path, output)
    for line in error:
      print line
    return TestResults.ConsoleResultTuple(valid, baseline_path, output, diff)

  def _validate_rendering(self, baseline_path='.', rendering_path='.'):
    """ Compare renderings """
    valid = False
    diff_path = ''
    error = []

    # Comparison thresholds
    fuzz= 6               # % of fuzz (increase to ignore small differences)
    error_threshold= 10   # Number of pixels that must differ in order to trigger a warning

    g= glob.glob(baseline_path)
    if g:
      baselinerendering= max(g, key= os.path.getmtime)
      try:
        process= subprocess.Popen(
          ["compare",
            "-fuzz","%d%%"%fuzz,
            "-metric","AE",
            baseline_path,rendering_path, os.path.split(rendering_path)[0] + '\\diff_' + os.path.split(rendering_path)[1]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
      except Exception as E:
        error.append("WARNING: Image comparison not available! Probably Imagemagick is not installed, or not in the path, or one of the following files do not exist:\n" +
                     "baseline: %s\noutput:%s\n" % (baseline_path,rendering_path) +
                     "Or the following file could not be created:\n" +
                     "diff: %s" % (os.path.split(rendering_path)[0] + '/diff_' + os.path.split(rendering_path)[1]))
        process= None

      if process:
        (output, err) = process.communicate()
        exit_code = process.wait()

        metric= None
        try:
          metric= int(err)
        except:
          valid = False;
          error.append('WARNING: Image comparison failed!\n\nDetails:\n%s\n%s' % (rendering_path, err) )

        if not metric is None and metric > error_threshold:
          valid = False
          error.append(
            'WARNING: The rendering ' +
            'is significantly different to the baseline. The differences are attached as ' + (os.path.split(rendering_path)[0] + '/diff_' + os.path.split(rendering_path)[1]) + '\n' )
          diff_path = ( (os.path.split(rendering_path)[0] + '/diff_' + os.path.split(rendering_path)[1]) )

        else:
          # Test passed, we can clean up the file right now
          valid = True
          try:
            os.remove ( diff_path )
          except:
            pass#print "WARNING: Could not remove " + diff_path

    else:
      pass#print "WARNING: No rendering baseline, skipping validation!"
    return valid, diff_path, error

  def _validate_text(self, baseline_path='.', output=''):
    try:
      valid = True
      diff = ''
      error = []
      differ = difflib.Differ()
      f = open(baseline_path, 'r')    
      baseline = [line.replace('\n', '') for line in f.readlines()]  
      f.close()
      output_list = [out.replace('\n', '') for out in output]
      delta = differ.compare(baseline, output_list)
      try:
        while True:
          line = delta.next()
          if line[0] != ' ':
            valid = False
          diff += line + '\n'
      except StopIteration:
        return valid, diff, error #This is where we end up if it worked
    except Exception as e:
        error.append(str(e))
    return False, diff, error


  def getNextLine(self, file):
    line = file.readline()
    while line and line.strip() == '':
      line = file.readline()
    return line.strip()

  def parseValidationFile(self, file_path='validation.txt', baseline_folder='', text_output_folder=''):
    self.step_list = []
    if (self.errors > 0):
      self.step_list.append(self.StepResultTuple("", False, [self.ErrorResultTuple(self.std_out + "\nTest returned errors", self.std_err)]))
    elif not self.terminates_ok:
      self.step_list.append(self.StepResultTuple("", False, [self.ErrorResultTuple(self.std_out + "\nTest didn't finish successfully (crashed, froze, or was otherwise interrupted.)", self.std_err)]))
    if not os.path.exists(file_path):
      self.step_list.append(self.StepResultTuple("", False, [self.ErrorResultTuple(self.std_out + "\nTest didn't output validation data, it probably crashed.", self.std_err)]))
      return
    
    
    f = open(file_path, 'r')
    step_name = self.getNextLine(f)
    while step_name != None:
      success = True
      line = self.getNextLine(f)
      if line == '':
        break;
      results = []
      while line != None:
        if line == 'screenshot':
          # Screenshot format: One line that says screenshot, one line with output screenshot path
          screenshot_path = self.getNextLine(f)
          res = self.getRenderingResult(baseline_folder + '\\' + os.path.split(screenshot_path)[1], screenshot_path)
          success = success and res.success
          results.append(res)
          
        elif line == 'performance':
          # Performance format: One line that says performance, one line with all framerate samples, separated by space
          fps_data = self.getNextLine(f)
          res = self.getPerformanceResult(fps_data)
          #success = success and res.success
          results.append(res)
        elif line == 'console':
          # Console format: one line that says console and nothing else. We need to get stderr and look through it for a line that says "console_start_<stepname>" and read it until we get to "console_end_<stepname>"
          
          output = []
          found = False
          for err_line in self.std_err.splitlines(False):
            if not found:
              if err_line == "console_start_" + step_name:
                found = True
            elif err_line == "console_end_" + step_name:
              break
            else:
              output.append(err_line + '\n')
          console_f = open(text_output_folder +'\\' + step_name + '_console.txt', 'w')
          console_f.writelines(output)
          console_f.flush()
          console_f.close()            
          res = self.getConsoleResult(baseline_folder + '\\' + step_name+"_console.txt", output)
          success = success and res.success
          results.append(res)
        elif line == 'custom_start':
          line = self.getNextLine(f)
          output = []
          while line != 'custom_end':
            output.append(line + '\n')
            line = self.getNextLine(f)
          res = self.getCustomResult(baseline_folder + '\\' + step_name+"_custom.txt", output)
          success = success and res.success
          results.append(res)
        else: # If we get anything else then we've reached the end of the current step and the start of the next one!
          step = TestResults.StepResultTuple(step_name, success, results)
          self.step_list.append(step)
          break # goes out to the while step_name != True loop,
        line = self.getNextLine(f)
      step_name = line
