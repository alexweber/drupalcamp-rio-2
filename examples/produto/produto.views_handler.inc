<?php
/**
 * @file
 * Handlers de Views para o módulo Produto.
 */

/**
 * Handler para campo de ver um produto.
 *
 * @ingroup views_field_handlers
 */
class produto_handler_view extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['pid'] = 'pid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Texto para mostrar'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $pid = $values->{$this->aliases['pid']};

    // Carrega a entidade.
    $entity = entity_load_single('produto', $pid);

    // Verifica o acesso.
    if (!produto_access('view', $entity)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $entity_uri = entity_uri('produto', $entity);
    return l($text, reset($entity_uri));
  }

}

/**
 * Handler para campo de editar um produto.
 *
 * @ingroup views_field_handlers
 */
class produto_handler_edit extends produto_handler_view {

  function render($values) {
    $pid = $values->{$this->aliases['pid']};
    $entity = entity_load_single('produto', $pid);
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $entity_uri = entity_uri('produto', $entity);
    return l($text, reset($entity_uri) . '/edit');
  }

}

/**
 * Handler para campo de excluir um produto.
 *
 * @ingroup views_field_handlers
 */
class produto_handler_delete extends produto_handler_view {

  function render($values) {
    $pid = $values->{$this->aliases['pid']};
    $entity = entity_load_single('produto', $pid);
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $entity_uri = entity_uri('produto', $entity);
    return l($text, reset($entity_uri) . '/delete');
  }

}

/**
 * Handler para campo de operações de produto.
 *
 * @ingroup views_field_handlers
 */
class produto_handler_operations extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['pid'] = 'pid';
    $this->additional_fields['type'] = 'type';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $pid = $values->{$this->aliases['pid']};

    $destination = drupal_get_destination();
    $operations = array();

    // Load the produto entity.
    $entity = entity_load_single('produto', $pid);
    $entity_uri = entity_uri('produto', $entity);

    // Add view link if user has permsission.
    if (produto_access('view', $entity)) {
      $operations['view'] = array(
        'title' => t('view'),
        'href' => reset($entity_uri),
        'destination' => $destination,
      );
    }

    // Add edit link if user has permsission.
    if (produto_access('edit', $entity)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => reset($entity_uri) . '/edit',
        'destination' => $destination,
      );
    }

    // Add delete link if user has permsission.
    if (produto_access('delete', $entity)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => reset($entity_uri) . '/delete',
        'destination' => $destination,
      );
    }

    return array(
      '#theme' => 'links__produto_operations',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline')),
    );
  }

}