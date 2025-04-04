<?php
class ItonicsNiralaFacultyController extends DrupalDefaultEntityController
{

  /**
   * Create and return a new itonics_nirala_faculty entity.
   */
  public function create()
  {
    $entity = new stdClass();
    $entity->type = 'itonics_nirala_faculty';
    $entity->id = 0;
    $entity->name = '';
    $entity->created = REQUEST_TIME;
    return $entity;
  }

  /**
   * Saves the entity using drupal_write_record().
   */
  public function save($entity)
  {
    if (empty($entity->id)) {
      $entity->created = time();
    }

    module_invoke_all('entity_presave', $entity, 'itonics_nirala_faculty');

    // Determine if it's an insert or update
    $primary_keys = $entity->id ? 'id' : array();

    drupal_write_record('itonics_nirala_faculty', $entity, $primary_keys);

    $invocation = empty($primary_keys) ? 'entity_insert' : 'entity_update';

    if (empty($primary_keys)) {
      field_attach_insert('itonics_nirala_faculty', $entity);
    } else {
      field_attach_update('itonics_nirala_faculty', $entity);
    }

    module_invoke_all($invocation, $entity, 'itonics_nirala_faculty');

    return $entity;
  }

  public function delete($id)
  {
    db_delete('itonics_nirala_faculty')
      ->condition('id', $id)
      ->execute();
    return TRUE;
  }
}